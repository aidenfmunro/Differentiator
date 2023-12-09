#define ADD_(left, right)  createFuncNode (ADD, left, right)
#define MUL_(left, right)  createFuncNode (MUL, left, right)
#define SUB_(left, right)  createFuncNode (SUB, left, right)
#define DIV_(left, right)  createFuncNode (DIV, left, right)
#define POW_(left, right)  createFuncNode (POW, left, right)

#define SIN_(value)        createFuncNode (SIN, NULL, value)
#define COS_(value)        createFuncNode (COS, NULL, value)
#define CTG_(value)        createFuncNode (CTG, NULL, value)
#define  TG_(value)        createFuncNode (TG,  NULL, value)
#define  LN_(value)        createFuncNode (LN,  NULL, value)

#define NUM_(value)        createConstNode (value, NULL, NULL)

#define VAR_(value)        createVarNode (value, NULL, NULL)