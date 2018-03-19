#define NKW 14// no. of key words
#define ALNG 20 // no. of significant chars in identifiers
#define LLNG 121 // input line length
#define KMAX 10 // max no. of significant digits
#define TMAX 500 // size of table
#define FMAX 20 // size of function table
#define AMAX 30 // size of array table
#define CSMAX 30 // max no. of cases
#define SMAX 1000 // size of string table
#define ERMAX 100 // max error no.
#define XMAX 32767 // 2^15 -1
#define NMAX 36767 // 2^31-1 max number
#define LINELENG 132 // output line length
#define LINELIMIT 200 // line limit
#define SYMNUM 38 // symbol num
#define MCMAX 2000 // size of middle code

typedef enum symbol{
	casesy, charsy, constsy, defaultsy, elsesy, ifsy, intsy, 
	mainsy, printfsy, returnsy, scanfsy, switchsy, voidsy, whilesy,
	plussy, minussy, 
	multisy, divisy, 
	lesssy, leoeqsy, gresy, groeqsy, neqsy, eqsy, // < <= > >= != ==
	lbracketsy, rbracketsy, lparentsy, rparentsy, // [ ] ( )
	lbracesy, rbracesy, semicolonsy, commasy, assignsy, colonsy, // { } ; , = :
	idsy,
	intcon, charcon, stringcon,
}symbol;

typedef enum objecttype{
	konstant, vvariable, funktion
}objecttyp;

typedef enum typetype{
	notyp, ints, chars, intarrays, chararrays
}types;

typedef struct tabitem{
	char name[ALNG + 1];
	int link;
	objecttyp obj;
	types typ;
	int ref;
	int findex;
	int adr;
}tabitem;

typedef struct atabitem{
	int len;  // length of the array
	int elsize; // size of the element of an array
	int size; // size of an array
}atabitem;

typedef struct ftabitem{
	int last;
	int lastpar;
	int psize;
	int vsize;
}ftabitem;

typedef struct erroritem{
	int errornum;
	int linenum;
	int charnum;
}erroritem;

extern char ch; // last character read from source program	
extern long long int inum; // integer from insymbol
extern int sleng; // string length
extern int cc; // character counter
extern int ll; // length of current line
extern int linenum; // line number
extern int errpos; // last error postion
extern int t, a, f, sx, e; // indices to tables: t: tab, a: atab, f: ftab, sx: stab, e: errortab
extern symbol sy; // last symbol read by insymbol
extern char token[ALNG+1]; // identifier from insymbol
extern int tkl; // length of token
extern char line[LLNG]; // one line source code
const char key[NKW][ALNG + 1] = {"case", "char", "const", "default", "else", "if", "int", "main", "printf", "return", "scanf", "switch", "void", "while"};
const symbol ksy[NKW] = {casesy, charsy, constsy, defaultsy, elsesy, ifsy, intsy, mainsy, printfsy, returnsy, scanfsy, switchsy, voidsy, whilesy};
extern tabitem tab[TMAX]; // identifier table
extern atabitem atab[AMAX]; // array table
extern ftabitem ftab[FMAX]; // function table
extern char stab[SMAX]; // string table
extern erroritem errortab[ERMAX]; // error table

extern FILE *fpin; // the file to be compiled
extern FILE *fpout; // the result file