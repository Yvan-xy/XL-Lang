#define ATTRIBUTE_UNUSED __attribute__((__unused__))

// make sure rule linked to executable
extern int HelloXY;
int HelloLinked = HelloXY;