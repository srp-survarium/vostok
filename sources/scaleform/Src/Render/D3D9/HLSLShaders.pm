
package Shaders::D3D9;

use Data::Dumper;

@ISA = qw/Shaders/;

our @PixelShaderProfiles = qw/ps_3_0/;
our @VertexShaderProfiles = qw/vs_3_0/;

sub PlatformNames { qw/D3D9/ }

sub new 
{ 
    # MaxBatches must be 24, as some batched shaders can contain 10 uniforms (4x4 mvp, 4x2 cxform, 2x4x2 texxform).
    # 24*10 = 240, we are only guaranteed 256 uniforms on PS 2.0.
    bless { MaxBatches => 24} 
}

# Ureg      -> Assign constant registers at compile time
# Batchfv   -> passes batch uniforms to fragment shader via interpolators, instead of using relative addressing in fragment.
# Bin       -> Supports a binary version of shaders.
# Uname     -> include the uniform names (as strings) in the shader descriptor.

sub Caps { 'Ureg Batchfv Bin Uname' }
sub DefaultOutMode { 'b'; }

sub GetSem
{
    my ($vt,$vn,$vs) = ($_[0]->{type},$_[0]->{name},$_[0]->{sem});
    
    # Uniforms shouldn't have semantics in D3D.
    if ($_[0]->{vary} eq 'uniform' )
    {
        return '';
    }
    
    if ($vs eq '') 
    {
        if ($vn =~ /^a?tc([0-1]*)/) { $vs = 'TEXCOORD'.(0+$1); }
        elsif ($vn =~ /color/)      { $vs = 'COLOR0'; }
        elsif ($vn =~ /factor/)     { $vs = 'COLOR1'; }
        elsif ($vn =~ /^v?pos/)     { $vs = 'POSITION'; }
        elsif ($vn =~ /batch/)      { $vs = 'COLOR1'; }
        elsif ($vn =~ /fucxadd/)    { $vs = 'TEXCOORD3'; }
        elsif ($vn =~ /fucxmul/)    { $vs = 'TEXCOORD4'; }
    }
  return $vs;
}

sub ParamD3D 
{
  $_[0] =~ s/(^|\W)vec([1-4]?)(\W|$)/\1float\2\3/g;
  $_[0] =~ s/(^|\W)mat([1-4])(\W|$)/\1float\2x\2\3/g;
  $_[0] =~ s/(^|\W)lowpf([1-4]?)(\W|$)/\1half\2\3/g;
}

sub Param
{
    my $self = $_[0];
    $_ = $_[1];
    
    if ($_->{type} =~ /^sampler(.*)$/) 
    {
        my $s = "sampler $_->{name} : register(s\$texn)".($_[2]?';':'');
        $_->{uofs} = $texn;
        return $s, 2, {qw/texn $$texn++/};
    }

    my ($vt,$vn,$vs) = ($_->{type},$_->{name},GetSem($_));
    $vt =~ s/vec/float/g;
    if (exists $_->{dim}) 
    {
        $vn .= "[$_->{dim}]";
    }

    if ($_[0]->{stage} eq 'v' && $_->{vary} eq 'varying') 
    { 
        $vt = "out $vt"; 
    }
    elsif ($_[0]->{stage} eq 'f' && $_->{vary} eq 'fragout') 
    { 
        $vt = "out $vt"; 
    }
    my $s = "$vt $vn";
    $s.= " : $vs" if $vs;
    if ($_->{vary} eq 'uniform') 
    {
        $s .= " : register(c\$unin)".($_[2]?';':'');
        ParamD3D($s);
        my ($size, $align, $unit) = $self->Shaders::VarSize($_);
        return $s, 1, {unin=> '$$unin+='. $size};
    }
    else 
    {
        $s .= ',' if $_[2];
        ParamD3D($s);
        return $s, 0;
    }
}

sub VarSize
{
	my ($this, $var) = @_;
	my ($size, $align, $unit, $elems) = $this->SUPER::VarSize($var);
	return ($size, $align, $unit < 4 ? 4 : $unit, $elems);
}

sub BuiltinParams
{
    # Every pixel shader must output a COLOR value.
    return ($Shaders::stage eq 'f' ? ({vary => 'fragout', type => 'float4', name => 'fcolor', sem => 'COLOR'}) : ());
}

sub BlockD3D 
{
  my ($this, $stage, $argsr, $b) = @_;

  ParamD3D($b);
  $b =~ s/(^|\W)([0-9]*\.[0-9]*)(\W)/\1\2f\3/g;
  $b =~ s/(^|\W)([0-9]*\.[0-9]*)(\W)/\1\2f\3/g; # this is needed
  $b =~ s/(^|\W)mix(\W)/\1lerp\2/g;
  $b =~ s/(^|\W)texture([1-3]D\W)/\1tex\2/g;
  $b =~ s/(^|\W)texture([1-3]D)Lod\s*\(([^,]+),([^,]+),([^\)]+)\)(\W)/\1tex\2lod\(\3, half4\(\4, 0.0f, \5\)\)\6/g; 
  $b =~ s/(^|\W)(?:float|half|int)[1-4] *\(([^,()]*)\)/\1\2/g;
  
  return $b;
}

sub Block
{
  my ($this, $stage, $argsr, $b) = @_;
  my %args = %{$argsr};

  $b = BlockD3D($this, $stage, $argsr, $b);
  ParamD3D($b);
  return $b;
}

sub FileExt
{
  return 'hlsl';
}

sub GetOutputs
{
    my $this = $_[0];
    print "$_[1]\n";
    if ($_[1] eq 'b') 
    {
        return [0, 

                sub {   return if Shaders::CheckOut(@_);
                        my $f = "$this->{dir}/$_[0].".$this->FileExt($stage);
                        open(OUT,">$f") || die $f;
                        print OUT $_[1];
                        close(OUT);
                        
                        my @profileList = ($_[0] =~ /^V/) ? @VertexShaderProfiles : @PixelShaderProfiles;
                        my $shaderCompiled = 0;
                        foreach my $prof (@profileList)
                        {
                            Shaders::OutputMessage( 2, "fxc.exe /nologo /E main /T $prof /Fh $f.h /Vn pBinary_$_[0] $f\n" );
                            my $resultCode = system("fxc.exe /nologo /E main /T $prof /Fh $f.h /Vn pBinary_$_[0] $f");
                            #my $resultCode = 0;
                            if (!$resultCode)
                            {
                                system("cat $f.h >> $this->{dir}/$this->{name}_BinaryShaders.cpp; rm -f $f.h");
                                Shaders::OutputMessage( 3, "---------------------------\n" . `cat $f` . "\n---------------------------\n");
                                $shaderCompiled = 1;
                                last;
                            }
                        }
                        $shaderCompiled or die "$f: compilation failed\n\nShader Text:\n" . `cat $f`;
                    },
                       
                sub {   
                        my $filename = "$this->{dir}/$this->{name}_BinaryShaders.cpp $this->{dir}/*.".$this->FileExt($stage);
                        `rm -f $filename`;
                        Shaders::OutputMessage(2, "Deleting $filename\n");
                        $this->OpenFiles('f');
                        my $dxdir = `cygpath -u '$ENV{'DXSDK_DIR'}'`;
                        chomp($dxdir);
                        $ENV{'PATH'} .= ":$dxdir/Utilities/bin/x86";
                        #$ENV{'PATH'} =~ s/[;:].*Xbox 360 SDK[^:;]+[;:]//gi; # remove XEDK path, because it might find FXC from there instead.
                        
                        `echo -e "namespace Scaleform { namespace Render { namespace D3D9 { \\r\\n" >> $this->{dir}/$this->{name}_BinaryShaders.cpp`;
                    },
                    
                sub { 
                        $this->WriteHeaders(); 
                        $this->CloseFiles('f'); 
                        `echo -e "}}}\\r\\n" >> $this->{dir}/$this->{name}_BinaryShaders.cpp`;
                    }
               ];
    }
  return $_[0]->Shaders::GetOutputs($_[1]);
}

sub GetAttrStruct {
    my ($Platform, $name, $stage) = @_;
    if ($stage eq 'v')
    {
        return "    struct ${name}AttrDesc\n".
               "    {\n".
               "        const char*   Name;\n".
               "        unsigned      Attr;\n".
               "        unsigned char Stream;\n".
               "        unsigned char Type;       /* D3DDECLTYPE */\n".
               "        unsigned char ByteSize;\n".
               "        unsigned char Usage;      /* D3DDECLUSAGE */\n".
               "        unsigned char UsageIndex; /* D3DDECLUSAGE_INDEX */\n".
               "    };\n";
    }
    return 0;
}

sub GetVAttrDesc {
    my ($Platform, $vt,$vn,$vs) = ($_[0], $_[1]->{type},$_[1]->{name},$_[1]->{sem});
    my $base = "\"$vn\", ";

    $vt =~ /([0-9])/;
    my $typeComponents = $1 ? $1 : 1;
    $base .= "$typeComponents";
    my $vertexType;
    my $podType;
    if ($vn =~ /^a?tc([0-9]*)/) 
    { 
        $base .= ' | VET_TexCoord'; 
        $vertexType = "D3DDECLTYPE_FLOAT2"; 
        $podType = "float"; 
        $typeComponents = 2;
    }
    elsif ($vn =~ /^a?color/)   
    { 
        $base .= ' | VET_Color';    
        $vertexType = "D3DDECLTYPE_D3DCOLOR"; 
        $podType = "UByte"; 
        $typeComponents = 4;
    }
    elsif ($vn =~ /^a?factor/)  
    { 
        $base .= ' | VET_Color | (1 << VET_Index_Shift)'; 
        $vertexType = "D3DDECLTYPE_UBYTE4"; 
        $podType = "UByte";
        $typeComponents = 4;
    }
    elsif ($vn =~ /^a?pos/)     
    { 
        $base .= ' | VET_Pos'; 
        $vertexType = "D3DDECLTYPE_SHORT2"; 
        $podType = "short";
        $typeComponents = 2;
    }
    elsif ($vn =~ /^v?batch$/)    
    { 
        $base .= ' | VET_Instance8'; 
        $vertexType = "D3DDECLTYPE_UBYTE4"; 
        $podType = "UByte";
        $typeComponents = 4;
    }

    # Store D3DDECL specific elements directly in the attributes. This way, we don't
    # need to guess at runtime what the components should be.
    
    $base .= ', 0, '; # Stream, currently always zero.
    
    # Type, ByteSize
    $base .= "$vertexType, sizeof($podType)*$typeComponents";
       
    # Usage, UsageIndex
    $vs = GetSem($_[1]);
    $vs =~ s/([0-9])//;
    $vsindex = $1 ? $1 : '0';
    $base .= ", D3DDECLUSAGE_$vs, $vsindex";    

    
    
    return $base;
}

sub WriteHeaders
{
	my %stages = qw/v Vertex f Frag/;
	foreach my $stage ('v','f') {
		foreach my $g (keys %Shaders::groups) 
        {
			my $Group = $Shaders::groups{$g};
			my @out = @{$Group->{$stage.'out'}};
			foreach (@out) 
            {
				if (ref($_)) 
                {
					my $fn = Shaders::GetFName($stage, @{$_->{blocks}});
					my $sym = "pBinary_${fn}";
					print Shaders::OUTC "extern const BYTE ${sym}[];\n";
				}
			}
		}
	}

        Shaders::WriteHeaders();
}

sub BinaryShader { return "pBinary_$_[2]" };

# just cast it in C.
#sub ShaderBinaryType { 'const DWORD*' } 

sub GetBatchVar 
{
    my $v = $_[1], $array = 'f';
    my $size = $v->{dim} ? $v->{dim} : 1;
  
    # In D3D, always make everything a float4 - if it comes in as a matrix
    if ($v->{type} =~ /(?:^mat)|([0-9])x([0-9])/) 
    {
        if ( $1 eq 'mat' ) 
        {
            $v->{type} = "float4x4";
        }
    }
    return ($array, $size);
}

new();
