#ifndef _T2S_H_
#define _T2S_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "regexp.h"

#define NDIPHS 3000
#define NFRAMES 23000

#define FW 1
#define CW 2
#define PUNCT 3

#define DEF_F0 125

#define _SPN_ 6
#define _MBROLA_ 7

#ifndef TRUE
#define FALSE (0)
#define TRUE (!0)
#endif

/* the following four should really come from the data header */
/* #define SR 10000	sample rate  */
/* #define FR_SZ 132	standard frame size  */
/* #define FR_DATA 16   shorts per frame, coeffs + assorted  */
/* #define NCOEFFS 12   ...  */

/* malloc defaults  */
#define DEF_BUFFER 1024
#define DEF_LING_LIST 100
#define DEF_SENT_LIST 100
#define DEF_SENT_SIL 100
#define DEF_SPL 100
#define DEF_PHONS 100
#define DEF_TARGS 100
#define DEF_FRAMES 100
#define DEF_PM 100

#define PHON_SZ 5
#define DIPH_SZ 10

#define OUT_BUF 2048

#define VOWEL 1	/* for syllabification essentially  */
#define FREEPHONE_CONST 2
#define BOUND 3
#define STRESS 4

/* various typedefs  */

class Dictionary;
typedef struct {
	char *input_file;
	char *output_file;
	char *diphone_file;
	char *hash_file;
	char *format;
	int type;	/* format by any other name  */
	FILE *ifd;
	FILE *ofd;
	FILE *xfd;
	FILE *dfd;
	void *db;
	int fw_num;
	int broad_cats_num;
	int dur0_num;
	int edin2sampa0_num;
	char prog[40];
	int sr;
	int fr_sz;
	int fr_data;
	int ncoeffs;
	int norm;		/* used for normalising output amplitude  */
	int ft_endian_loc;	/* for telling byte order  */
	/* Added JACS: handle of phoneme database */
	Dictionary* phoneme_db;
} CONFIG;

typedef struct {
  int size;
  int fill;
  char *text;
} BUFFER;

/*
typedef struct {
	int max;
	int sz;
	char *ptr;
} BUFFER;
*/

typedef struct {
	char *word;
	int type;
	char *transcription;
} LING;

typedef struct {
	int max;
	int sz;
	LING **text;
} LING_LIST;

typedef struct key {
	char *keyword;
	int keycount;
} KEY;

typedef struct {
	char *keyword;
	char *keyvalue;
} PKEY;

typedef struct {
        char *shape;
	float factor;
	int durn;
} P_ELEM;
         

typedef struct {
  int sil_sz;
  int sil_max;
  P_ELEM *sil;
  BUFFER list;
} SENT;

typedef struct {
	char phoneme[5];
	int syll;
	int dur;
	char *sprosod1;
	char *sprosod2;
	float strength1;
	float strength2;	/* for combined elements  */
} SPROSOD;

typedef struct {
	int max;
	int sz;
	SPROSOD **phoneme;
} SPROSOD_LIST;

typedef struct {
        char diph[10];
        int beg;
        int mid;
        int end;
} ENTRY;

typedef struct {
        short *frame; 
} FRAME;

typedef struct {
	int p_sz;
	int p_max;
	int t_sz;
	int t_max;
	char **phons;
	int *duration;
	int *cum_dur;
	int *pc_targs;
	int *targ_phon;
	int *targ_freq;
	int *abs_targ; /* maybe in samples  */
	int *pb;
	float *scale;
	char **diphs;
} SPN;

typedef struct {
	int f_sz;
	int p_sz;
	int f_max;
	int p_max;
	FRAME **mcebuf;
	short *duration; /* since variants may be required  */
	short *pitch;
} ACOUSTIC;

typedef struct {
  char *left_context;
  regexp *lc;
  char *target;
  char *right_context;
  regexp *rc;
  char *output;
} RULE;

#define SAME 0x0001

typedef char *Rule[4];

extern KEY fw[];
extern KEY broad_cats[];
extern KEY dur0[];
extern PKEY edin2sampa0[];

/* now definitions of global data  */

extern ENTRY indx[NDIPHS]; 
extern FRAME dico[NFRAMES];
extern int nindex;
extern char *dbName;	/* ???  */

/* Endian and byte swaping */
extern int ft_endian_loc;
/* Sun, HP, SGI Mips, M68000 */
#define FT_BIG_ENDIAN (((char *)&ft_endian_loc)[0] == 0)
/* Intel, Alpha, DEC Mips, Vax */
#define FT_LITTLE_ENDIAN (((char *)&ft_endian_loc)[0] != 0)
 
#define SWAPINT(x) (((x) & 0xff) << 24 | ((x) & 0xff00) << 8 | \
                    ((x) & 0xff0000) >> 8 | ((x) & 0xff000000) >> 24)
#define SWAPSHORT(x) (((x) & 0xff) << 8 | ((x) & 0xff00) >> 8)


// Represents the entire FreePhone implementation
class wxFreePhone;
class tsFreePhoneImplementation
{
    friend class wxFreePhone;
public:
// Constructor and destructor
    tsFreePhoneImplementation();
    ~tsFreePhoneImplementation();

// Accessors
    CONFIG& GetConfig() { return config_struct; }

// Implementation

    // audio.cpp
    void output_open(CONFIG *config);
    void output_close(CONFIG *config);

    // buffer.cpp
    char *buffer_text(BUFFER *buffer);
    void buffer_init(BUFFER *buffer);
    void buffer_require(BUFFER *buffer, int space);
    void buffer_add_char(BUFFER *buffer, int c);
    void buffer_add_str(BUFFER *buffer, char *str);
    char buffer_last_char(BUFFER *buffer);
    void buffer_clear(BUFFER *buffer);
    void buffer_free(BUFFER *buffer);

    // conv1.cpp
    void conv1(CONFIG *config, LING_LIST *ling_list, SENT *sent);
    void sil_fill(int j, char *shape, float factor, int durn, SENT *sent);

    // conv2.cpp
    void conv2(CONFIG *config, SENT *sent, SPROSOD_LIST *spl);
    int vowel(char *ph);
    void spl_cpy(int index,int syll, char *phon, int dur, char *type, float strength, SPROSOD_LIST *spl);
    void spl_cat(int index,char *type, float strength, SPROSOD_LIST *spl);

    // durations.cpp
    void durations(CONFIG *config, SPN *ps, ACOUSTIC *as);

    // durpros.cpp
    void durpros(CONFIG *config, SPROSOD_LIST *spl);

    // go.cpp
    void go(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps, ACOUSTIC *as);
    void go3(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps);
    void go2(CONFIG *config, SPN *ps, ACOUSTIC *as);
    void zero_things(BUFFER *buffer, LING_LIST *ling_list);

    // grammar.cpp
    void grammar(LING_LIST *ling_list);

    // interface.cpp
    char *nrl_rules(char *in);
    char new_char();
    void xlate_string(char* is);
    void have_dollars();
    void have_special();
    void have_number();
    void have_letter();
    void abbrev(char buff[]);
    void say_cardinal(long int);
    void say_ordinal(long int value);
    void say_ascii(int character);
    void spell_word(char* word);
    int makeupper(int character);
    int Xgetc();

    // load_diphs.cpp
    void load_speech(CONFIG *config);
    int lookup(char *diph);
    void phonstoframes(SPN *ps, ACOUSTIC *as);
    void unload_diphs(CONFIG *config);
    void load_index(CONFIG *config);
    void load_diphs(CONFIG *config);
    int read_header(CONFIG *config);

    // make_wave.cpp
    void make_wave(CONFIG *config, ACOUSTIC *as);

    // nrl_edin.cpp
    void nrl_edin_conv(char *str, char *str2);

    // phon_rules.cpp
    void phon_rules_init();
    void phon_rules_free();
    void phon_rules(SENT *sent);
    void load_context_rules(char * filename);

    // phoneme.cpp
    void outstring(char* string);
    int find_rule(char word[], int index, Rule* rules);
    int leftmatch(char* pattern, char* context);
    int rightmatch(char* pattern, char* context);
    int isvowel(char chr);
    int isconsonant(char chr);
    void xlate_word(char word[]);

    // pitch.cpp
    void calc_pitch(CONFIG *config, SPN *ps, ACOUSTIC *as);
    int interpolated_freq(int k, SPN *ps);
    int interpolate(int a,int b,int c,int d,int e);

    // prosody.cpp
    void prosody(SPROSOD_LIST *spl, SPN *ps);
    void set_target(int i, int freq, int perc, SPN *ps);

    // read_rules.cpp
    int read_rule_file(char *filename, RULE *rules);
    char *conv_style(char * style);
    char *conv_dirn(char * direction);
    void process_rule(char *input,RULE *rule);
    void replace_classes(int num_rules, RULE *rules, int num_classes, char **key, char **value);
    void replace_class(char *buffer, char *ptr, char *key, char *value);
    char *apply_class(int num_classes, char *string, char **key, char **value);
    void use_class(char *buffer, char *key, char *value);

    // space.cpp
    void init(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps, ACOUSTIC *as);
    void terminate(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps, ACOUSTIC *as);
    void ling_list_malloc(int num, LING_LIST *ling_list);
    void ling_list_realloc(int num, LING_LIST *ling_list);
    void ling_list_free(LING_LIST *ling_list);
    void spl_malloc(int num, SPROSOD_LIST *spl);
    void spl_realloc(int num, SPROSOD_LIST *spl);
    void spl_free(SPROSOD_LIST *spl);
    void ps_malloc(int nphons, int ntargs, SPN *ps);
    void ps_realloc(int nphons, int ntargs, SPN *ps);
    void ps_free(SPN *ps);
    void as_malloc(int nframes, int npp, ACOUSTIC *as);
    void as_realloc(int nframes, int npp, ACOUSTIC *as);
    void as_free(ACOUSTIC *as);
    void sent_init(SENT *sent);
    void sent_alloc_sil(SENT *sent, int n);
    void sent_free(SENT *sent);

    // spnio.cpp
    void put_mbrola_data(CONFIG *config, SPN *ps);
    void put_spn_data(CONFIG *config, SPN *ps);
    void get_spn_data(CONFIG *config, SPN *ps);
    void transmogrify(CONFIG *config, char *s, SPN *ps);

    // syllab.cpp
    char *syllabify(char *string, CONFIG *config);
    char *stress(char *input);
    int b_cat(char *ascii, CONFIG *config);
    int heavy(char *phon);
    int seq(char * prev, char * curr);

    // t2s.cpp
    void process_sentence(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps, ACOUSTIC *as);
    void produce_spn_file(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps);
    void process_spn_file(CONFIG *config, SPN *ps, ACOUSTIC *as);
    void high_level(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps);
    void low_level(CONFIG *config, SPN *ps, ACOUSTIC *as);

    // tags.cpp
    void tags(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list);
    int in_fw_list(char *ascii, CONFIG *config);
    int something_to_say(char *ascii);

    // transcribe.cpp
    void transcribe(CONFIG *config, LING_LIST *ling_list);
    char *lookup_db(char *word, CONFIG *config);
    void ToLower(char *word);

    // rule_engine.cpp
    void rule_exec(int type,int nrules, RULE *rule, SENT* input);

    // utils.cpp
    char **split(char *in);
    void tidy_split(char **root);
    KEY *binary(char *word, KEY tab[], int n);
    PKEY *pbinary(char *word, PKEY tab[], int n);

// Members
protected:
    CONFIG                  config_struct;
    BUFFER                  buffer_struct;
    LING_LIST               ling_list_struct;
    SENT                    sent_struct;
    SPROSOD_LIST            spl_struct;
    SPN                     ps_struct;
    ACOUSTIC                as_struct;
};

#endif
    // _T2S_H_
