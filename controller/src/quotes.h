#include <Arduino.h>

// struct Movie {
//   const char *title;
//   const char *lines[7];
// };

// const char empty_line[] PROGMEM = "";

// const char movie_1_0[] PROGMEM = " RED 2";
// const char movie_1_1[] PROGMEM = " Frank: Is that a stick of dynamite in";
// const char movie_1_2[] PROGMEM = " your pocket?";
// const char movie_1_3[] PROGMEM = " Marvin: Yeah, but I'm saving it for";
// const char movie_1_4[] PROGMEM = " emergency.";

// PGM_P const movie_1[8] PROGMEM = { movie_1_0, movie_1_1, movie_1_2, empty_line, movie_1_3, movie_1_4, empty_line, empty_line };

// const char movie_2_0[] PROGMEM = " VALKYRIE";
// const char movie_2_1[] PROGMEM = " Any problem on Earth can be solved";
// const char movie_2_2[] PROGMEM = " with the careful application of high";
// const char movie_2_3[] PROGMEM = " explosives. The trick is not to be";
// const char movie_2_4[] PROGMEM = " around when they go off.";
// const char movie_2_5[] PROGMEM = "";
// const char movie_2_6[] PROGMEM = "";
// const char movie_2_7[] PROGMEM = "";

// PGM_P const movie_2[8] PROGMEM = {
//   movie_2_0, movie_2_1, movie_2_2, movie_2_3, movie_2_4, movie_2_5, movie_2_6, movie_2_7
// };

// const char movie_3_0[] PROGMEM = " DIE HARD";
// const char movie_3_1[] PROGMEM = " John McClane: Why'd you have to nuke";
// const char movie_3_2[] PROGMEM = " the whole building, Hans?";
// const char movie_3_3[] PROGMEM = " Hans Gruber: Well, when you steal $600";
// const char movie_3_4[] PROGMEM = " you can just disappear. When you steal";
// const char movie_3_5[] PROGMEM = " 600 million, they will find you unless";
// const char movie_3_6[] PROGMEM = " they think you're already dead.";

// PGM_P const movie_3[8] PROGMEM = {
//   movie_3_0, movie_3_1, movie_3_2, empty_line, movie_3_3, movie_3_4, movie_3_5, movie_3_6
// };

// const char movie_4_0[] PROGMEM = " SPEED";
// const char movie_4_1[] PROGMEM = " Pop quiz, hotshot. There's a bomb on a";
// const char movie_4_2[] PROGMEM = " bus. Once the bus goes 50 miles an";
// const char movie_4_3[] PROGMEM = " hour the bomb is armed. If it drops";
// const char movie_4_4[] PROGMEM = " below 50 it blows up. What do you do?";
// const char movie_4_5[] PROGMEM = " What do you do?!";

// PGM_P const movie_4[8] PROGMEM = {
//   movie_4_0, movie_4_1, movie_4_2, movie_4_4, movie_4_4, movie_4_5, empty_line, empty_line
// };

// const char movie_5_0[] PROGMEM = " DIE HARD WITH A VENGEANCE";
// const char movie_5_1[] PROGMEM = " John McClane: Hey, I know as much as";
// const char movie_5_2[] PROGMEM = " you know, all right? There's a guy out";
// const char movie_5_3[] PROGMEM = " there setting off bombs. He calls";
// const char movie_5_4[] PROGMEM = " himself Simon. He speaks with a German";
// const char movie_5_5[] PROGMEM = " accent. And for some reason, he's very";
// const char movie_5_6[] PROGMEM = " angry with me.";

// PGM_P const movie_5[8] PROGMEM = {
//   movie_5_0, movie_5_1, movie_5_2, movie_5_5, movie_5_5, movie_5_5, movie_5_6, empty_line
// };

// PGM_P const * const shows[8] PROGMEM = {
//   movie_1, movie_2, movie_3, movie_4
// };


// const char code_1[] PROGMEM = "static int ssl3_generate_key_block";
// const char code_2[] PROGMEM = "(SSL_CONNECTION *s, unsigned char *km, int num)";
// const char code_3[] PROGMEM = "{";
// const char code_4[] PROGMEM = "    const EVP_MD *md5 = NULL, *sha1 = NULL;";
// const char code_5[] PROGMEM = "    EVP_MD_CTX *m5;";
// const char code_6[] PROGMEM = "    EVP_MD_CTX *s1;";
// const char code_7[] PROGMEM = "    unsigned char buf[16], smd[SHA_DIGEST_LENGTH];";
// const char code_8[] PROGMEM = "    unsigned char c = 'A';";
// const char code_9[] PROGMEM = "    unsigned int i, k;";
// const char code_10[] PROGMEM = "    int ret = 0;";
// const char code_11[] PROGMEM = "    SSL_CTX *sctx = SSL_CONNECTION_GET_CTX(s);";
// const char code_12[] PROGMEM = "";
// const char code_13[] PROGMEM = "#ifdef CHARSET_EBCDIC";
// const char code_14[] PROGMEM = "    c = os_toascii[c];          /* 'A' in ASCII */";
// const char code_15[] PROGMEM = "#endif";
// const char code_16[] PROGMEM = "    k = 0;";
// const char code_17[] PROGMEM = "    md5 = ssl_evp_md_fetch(sctx->libctx, NID_md5, sctx->propq);";
// const char code_18[] PROGMEM = "    sha1 = ssl_evp_md_fetch(sctx->libctx, NID_sha1, sctx->propq);";
// const char code_19[] PROGMEM = "    m5 = EVP_MD_CTX_new();";
// const char code_20[] PROGMEM = "    s1 = EVP_MD_CTX_new();";
// const char code_21[] PROGMEM = "    if (md5 == NULL || sha1 == NULL || m5 == NULL || s1 == NULL) {";
// const char code_22[] PROGMEM = "        SSLfatal(s, SSL_AD_INTERNAL_ERROR, ERR_R_EVP_LIB);";
// const char code_23[] PROGMEM = "        goto err;";
// const char code_24[] PROGMEM = "    }";
// const char code_25[] PROGMEM = "    for (i = 0; (int)i < num; i += MD5_DIGEST_LENGTH) {";
// const char code_26[] PROGMEM = "        k++;";
// const char code_27[] PROGMEM = "        if (k > sizeof(buf)) {";

// PGM_P const code[] PROGMEM =
// {
//   code_1,
//   empty_line,
//   code_2,
//   code_3,
//   code_4,
//   code_5,
//   code_6,
//   code_7,
//   code_8,
//   code_9,
//   code_10,
//   code_11,
//   code_12,
//   code_13,
//   code_14,
//   code_15,
//   code_16,
//   code_17,
//   code_18,
//   code_19,
//   code_20,
//   code_21,
//   code_22,
//   code_23,
//   code_24,
//   code_25,
//   code_26,
//   code_27
// };
