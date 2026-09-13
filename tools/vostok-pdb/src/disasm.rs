//! Thin 32-bit x86 disassembly layer over `iced-x86` for the rich-context
//! listing.
//!
//! Two jobs:
//!   * decode a function's `.text` bytes into a flat instruction list, and
//!   * discover branch targets that land *inside* the function so they can be
//!     given short local labels (`.1`, `.2`, …) — the same readability trick the
//!     hand-written brief uses.
//!
//! Instruction operands are rendered through a [`SymbolResolver`] that rewrites
//! in-function branch targets to their local label and call/data targets to the
//! symbol names recovered from the PDB (see [`crate::rich_context::SymbolMaps`]),
//! so a `jg` reads as `jg .2` and a `call` as `call vostok::foo::bar` directly in
//! the operand text.
//!
//! The decode loop mirrors `vostok-delinker`'s `resolve_relative_relocations`
//! (same crate, same flow-control checks), but here we only *read* the original
//! in-place EXE, so branch displacements are already correct absolute VAs and no
//! relocation patching is needed.

use std::collections::BTreeSet;
use std::collections::HashMap;
use std::rc::Rc;

use iced_x86::{
    Decoder, DecoderOptions, FlowControl, Formatter as _, Instruction, IntelFormatter, OpKind,
    SymbolResolver, SymbolResult,
};

use crate::rich_context::SymbolMaps;

pub struct DecodedFunction {
    pub instructions: Vec<Instruction>,
    /// VA -> local label (e.g. `.1`) for branch targets inside the function,
    /// assigned in ascending-address order.
    pub labels: HashMap<u64, String>,
}

/// Decode `code` (32-bit x86) whose first byte is at virtual address `va_base`.
pub fn decode(code: &[u8], va_base: u64) -> DecodedFunction {
    let mut decoder = Decoder::with_ip(32, code, va_base, DecoderOptions::NONE);

    let mut instructions = Vec::new();
    while decoder.can_decode() {
        instructions.push(decoder.decode());
    }

    let end = va_base + code.len() as u64;

    let mut targets = BTreeSet::new();
    for insn in &instructions {
        if !matches!(
            insn.flow_control(),
            FlowControl::ConditionalBranch | FlowControl::UnconditionalBranch
        ) {
            continue;
        }
        let target = match insn.op0_kind() {
            OpKind::NearBranch16 | OpKind::NearBranch32 | OpKind::NearBranch64 => {
                insn.near_branch_target()
            }
            _ => continue,
        };
        if (va_base..end).contains(&target) {
            targets.insert(target);
        }
    }

    let labels = targets
        .into_iter()
        .enumerate()
        .map(|(i, va)| (va, format!(".{}", i + 1)))
        .collect();

    DecodedFunction {
        instructions,
        labels,
    }
}

/// Resolves operand addresses to local labels and recovered symbol names.
///
/// Must be `'static` (iced takes a `Box<dyn SymbolResolver>`); it therefore owns
/// the per-function label map and an `Rc` to the shared symbol maps.
struct Resolver {
    image_base: u64,
    labels: HashMap<u64, String>,
    symbols: Rc<SymbolMaps>,
}

impl SymbolResolver for Resolver {
    fn symbol(
        &mut self,
        _instruction: &Instruction,
        _operand: u32,
        _instruction_operand: Option<u32>,
        address: u64,
        _address_size: u32,
    ) -> Option<SymbolResult<'_>> {
        if let Some(label) = self.labels.get(&address) {
            return Some(SymbolResult::with_string(address, label.clone()));
        }

        let rva = address.checked_sub(self.image_base)? as usize;
        if let Some(name) = self.symbols.functions.get(&rva) {
            return Some(SymbolResult::with_string(address, name.clone()));
        }
        if let Some(name) = self.symbols.data.get(&rva) {
            return Some(SymbolResult::with_string(address, name.clone()));
        }
        None
    }
}

/// Build an Intel-syntax formatter wired to the symbol resolver. `labels` is the
/// (cloned) per-function label map; `symbols` is shared via `Rc`.
pub fn make_formatter(
    image_base: u64,
    labels: HashMap<u64, String>,
    symbols: Rc<SymbolMaps>,
) -> IntelFormatter {
    let resolver = Resolver {
        image_base,
        labels,
        symbols,
    };

    let mut formatter = IntelFormatter::with_options(Some(Box::new(resolver)), None);
    let options = formatter.options_mut();
    // Align operands into a column so the listing reads like the brief example
    // (`mov   rbp, rsp`).
    options.set_first_operand_char_index(6);
    options.set_space_after_operand_separator(true);
    formatter
}
