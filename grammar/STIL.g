grammar STIL;

///////////////////////////////////////////////////////////////////////
// RULES
///////////////////////////////////////////////////////////////////////

program : format? header? signals signal_groups timing scan_structures_l
          pattern_burst_l pattern_exec_l procedures_l macros_l pattern_l EOF;

///////////////////////////////////////////////////////////////////////

format  : 'STIL' float_t (L_BRACKET design R_BRACKET)?;
design  : 'Design' int_t;

///////////////////////////////////////////////////////////////////////

header  : 'Header' L_BRACKET title? date? source? history? R_BRACKET;
title   : 'Title' STRING;
date    : 'Date' STRING;
source  : 'Source' STRING;
history : 'History' L_BRACKET R_BRACKET;

///////////////////////////////////////////////////////////////////////

signals             : 'Signals' L_BRACKET signal* R_BRACKET;
signal              : id signal_dir signal_attributes?;
signal_dir          : 'In' | 'Out' | 'InOut' | 'Pseudo';
signal_attributes   : L_BRACKET signal_scan? wfc_map? R_BRACKET;
signal_scan         : 'ScanIn' | 'ScanOut';
wfc_map             : 'WFCMap' L_BRACKET map_rule* R_BRACKET;
map_rule            : wfc_seq '->' wfc_seq;

///////////////////////////////////////////////////////////////////////

signal_groups   : 'SignalGroups' L_BRACKET signal_group* R_BRACKET;
signal_group    : id EQ QUOTE signal_list QUOTE signal_attributes?;
signal_list     : id (SUM id)*;

///////////////////////////////////////////////////////////////////////

timing          : 'Timing' L_BRACKET waveform_table* R_BRACKET;
waveform_table  : 'WaveformTable' id L_BRACKET period waveforms R_BRACKET;
period          : 'Period' time_expr;
waveforms       : 'Waveforms' L_BRACKET waveform* R_BRACKET;
waveform        : id L_BRACKET wfc_seq L_BRACKET event+ R_BRACKET R_BRACKET;
event           : time_expr event_code;

///////////////////////////////////////////////////////////////////////

scan_structures_l   : scan_structures*;
scan_structures     : 'ScanStructures' id? L_BRACKET scan_chain* R_BRACKET;
scan_chain          : 'ScanChain' id L_BRACKET scan_length scan_in scan_out
                      scan_inversion scan_cells scan_clock R_BRACKET;
scan_length         : 'ScanLength' int_t;
scan_in             : 'ScanIn' id;
scan_out            : 'ScanOut' id;
scan_inversion      : 'ScanInversion' int_t;
scan_cells          : 'ScanCells' (id EXCLAMATION?)+;
scan_clock          : 'ScanMasterClock' id?;

///////////////////////////////////////////////////////////////////////

pattern_burst_l : pattern_burst+;
pattern_burst   : 'PatternBurst' id L_BRACKET context pattern_list R_BRACKET;
context         : macro_context? proced_context?;
macro_context   : 'MacroDefs' id;
proced_context  : 'Procedures' id;
pattern_list    : 'PatList' L_BRACKET (pattern_call)* R_BRACKET;
pattern_call    : id (L_BRACKET context R_BRACKET)?;

///////////////////////////////////////////////////////////////////////

pattern_exec_l      : pattern_exec+;
pattern_exec        : 'PatternExec' id? L_BRACKET pattern_burst_call* R_BRACKET;
pattern_burst_call  : 'PatternBurst' id;

///////////////////////////////////////////////////////////////////////

procedures_l    : procedures*;
procedures      : 'Procedures' id? L_BRACKET procedure* R_BRACKET;
procedure       : id L_BRACKET inst_list R_BRACKET;

///////////////////////////////////////////////////////////////////////

macros_l    : macros*;
macros      : 'MacroDefs' id? L_BRACKET macro* R_BRACKET;
macro       : id L_BRACKET inst_list R_BRACKET;

///////////////////////////////////////////////////////////////////////

pattern_l   : pattern*;
pattern     : 'Pattern' id L_BRACKET inst_list R_BRACKET;


///////////////////////////////////////////////////////////////////////

inst_list   : inst*;
inst        : label? (loop | shift | w_inst | c_inst | f_inst | v_inst
            | call_inst | macro_inst | stop_inst | iddq_inst);
label       : id ':';
loop        : 'Loop' int_t L_BRACKET inst_list R_BRACKET;
shift       : 'Shift' L_BRACKET inst_list R_BRACKET;
w_inst      : 'W' id;
c_inst      : 'C' L_BRACKET assigs R_BRACKET;
f_inst      : 'F' L_BRACKET assigs R_BRACKET;
v_inst      : 'V' L_BRACKET assigs R_BRACKET;
call_inst   : 'Call' id (L_BRACKET assigs R_BRACKET)?;
macro_inst  : 'Macro' id (L_BRACKET assigs R_BRACKET)?;
stop_inst   : 'Stop';
iddq_inst   : 'IddqTestPoint';

assigs      : assig*;
assig       : id EQ assig_expr;
assig_expr  : JOIN? (repeat | wfc_seq)*;
repeat      : REPEAT int_t wfc_seq;

///////////////////////////////////////////////////////////////////////
// This is ugly but necessary, since the lexer doesn't know how to differentiate
// different tokens with some intersection. For example, the lexical definition
// of ID intersects with the definition of WFC, or events... Depending on the
// context there isn't ambiguity, but in the lexical analysis there isn't any context.
// To simplify and avoid some cases, we're going to consider that ALL ids are strings

id          : STRING;
num         : int_t | float_t;
int_t       : INT;
float_t     : FLOAT;
wfc_seq     : (CHARS | INT | WFC_SEQ);
event_code  : CHARS;
time_expr   : TIME_EXPR;

///////////////////////////////////////////////////////////////////////
// TOKENS
///////////////////////////////////////////////////////////////////////

SUM         : '+';
EQ          : '=';
SEMICOLON   : ';';
L_BRACKET   : '{';
R_BRACKET   : '}';
L_PAR       : '(';
R_PAR       : ')';
EXCLAMATION : '!';
QUOTE       : '\'';
JOIN        : '\\j';
REPEAT      : '\\r';

FLOAT   : INT'.'INT;
INT     : DIG+;
CHARS   : LETTER+;
STRING  : '"' ~('\r' | '\n' | '"')* '"';
WFC_SEQ : (DIG | LETTER | '#' | '%')+;

TIME_EXPR       : QUOTE (INT | FLOAT) UNIT QUOTE;
fragment UNIT   : 'ns' | 'ms' | 's';

fragment DIG    : [0-9];
fragment NUM    : INT | FLOAT;
fragment LETTER : [a-zA-Z];

// Ignored tokens (Order is important)
WHITE_SPACES    : [ \t\r\n;]+ -> skip;
COMMENT         : '//' ~('\r' | '\n')* -> skip;
ANNOTATION      : 'Ann ' L_BRACKET '*' ([ \p{S}\p{P}\p{M}\p{L}\p{N}])* '*' R_BRACKET -> skip;