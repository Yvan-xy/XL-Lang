#define ATTRIBUTE_UNUSED __attribute__((__unused__))

// make sure rule linked to executable
extern int HelloXY;
extern int BlockCalculate;
extern int BlockMerge;

int HelloLinked          = HelloXY;
int BlockCalculateLinked = BlockCalculate;
int BlockMergeLinked     = BlockMerge;

