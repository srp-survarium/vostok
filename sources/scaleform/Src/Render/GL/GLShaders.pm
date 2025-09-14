
package Shaders::GL;

@ISA = qw/Shaders/;

sub new { bless {nsname => 'GL', MaxBatches => 30} }

sub PlatformNames { qw/GL GLES/ }

sub Caps           { 'UnameBatchfv RemoveTemporaries'; }

sub GLESType
{
  $_[0] =~ s/(half)([1-4])x\2/mediump mat\2/g;
  $_[0] =~ s/(lowpf)([1-4])x\2/lowp mat\2/g;

  $_[0] =~ s/(^|\W)half([1-4])(\W|$)/\1mediump vec\2\3/g;
  $_[0] =~ s/(^|\W)lowpf([1-4])(\W|$)/\1lowp vec\2\3/g;
  $_[0] =~ s/(^|\W)half(\W|$)/\1mediump float\2/g;
  $_[0] =~ s/(^|\W)lowpf(\W|$)/\1lowp float\2/g;
  return $_[0];
}

sub Param
{
  my $self = $_[0];
  $_ = $_[1];

  return '', 0 if $_->{name} eq 'vpos';
  my $vt = $_->{type};

  if ($_[0]->{name} eq 'GLES') {
    $vt = GLESType($vt);
  }

  Shaders::ParamGL($vt);
  
  # Hack around atc bug, GLSL can't have Fragment attributes though, so change the attribute to a varying.
  my $s;
  if ( $_[0]->{stage} eq 'f' && $_->{vary} eq 'attribute' )
  {
    $s = "varying $vt $_->{name}";
  }
  else
  {
    $s = "$_->{vary} $vt $_->{name}";
  }
  $s .= "[$_->{dim}]" if ($_->{dim});
  $s .= ';' if $_[2];

  return $s, 1;
}

sub Block
{
  my ($this, $stage, $argsr, $b) = @_;
  my %args = %{$argsr};
  $Shaders::vpos = 'vpos';

  foreach (sort(values %args)) {
    if (lc($_->{sem}) eq 'position') {
      $vpos .= '|\Q'.$_->{name}.'\E';
    }
  }

  if ($this->{name} eq 'GLES') {
  	$b =~ s/(^|\W)lowpf([1-4])( +\w+ *(?:= |;))/\1lowp vec\2\3/g;
  	$b =~ s/(^|\W)lowpf( +\w+ *(?:= |;))/\1lowp float\2/g;
  }
  $b = Shaders::BlockGL($this, $stage, $argsr, $b);
  $b =~ s/(\Wmul) *\($Shaders::subexpr,$Shaders::subexpr\)/(\2) * (\3)/g;
  $b =~ s/((?:^|[^\w\.])[0-9])([^\w\.\(])/\1.0\2/g;
  $b =~ s/((?:^|[^\w\.])[0-9])([^\w\.\(])/\1.0\2/g;
  $b =~ s/(^|\W)saturate *\($Shaders::subexpr\)/\1clamp(\2,0.0,1.0)/go;

  if ($this->{name} eq 'GLES') {
    $b =~ s/(^|\W)texture2DLod *\($Shaders::subexpr,$Shaders::subexpr,$Shaders::subexpr\)/\1texture2D(\2,\3)/go;
    #$b =~ s/int\(([0-9+\.-]+)\)/{my $r = $1; $r =~ s/\.}/ge;
  }
  $b =~ s/\[$Shaders::subexpr\]/[int(0.1+\1)]/g;
  return $b;
}

sub BeginUniforms { (($_[0]->{stage} eq 'f' && $_[0]->{name} eq 'GLES') ? "precision mediump float;\n" : ''); }

sub BuiltinType   { return (($_[0]->{name} eq 'GLES') ? 'lowpf4' : 'float4'); }

sub Finish
{
  my ($this, $stage, $argsr) = @_;
  my $b = '';
  if ($stage eq 'f') {
    #$b.="  gl_FragColor = fcolor;\n";
  }
  return $b;
}

sub FileExt
{
  return 'glsl';
}

new();
