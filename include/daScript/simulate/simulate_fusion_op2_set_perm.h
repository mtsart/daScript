#define IMPLEMENT_VEC_SETOP(CTYPE) \
    IMPLEMENT_ANY_SETOP_VALUE(,CTYPE)

#define REGISTER_SETOP(OPNAME,TYPE,CTYPE) \
    (*g_fusionEngine)[fuseName(#OPNAME,typeName<CTYPE>::name())].push_back(make_shared<FusionPoint_Set_##OPNAME##_##CTYPE>());

#define IMPLEMENT_SETOP_INTEGER(OPNAME) \
    IMPLEMENT_ANY_SETOP(__forceinline,OPNAME,Int,int32_t); \
    IMPLEMENT_ANY_SETOP(__forceinline,OPNAME,UInt,uint32_t); \
    IMPLEMENT_ANY_SETOP(_msc_inline_bug,OPNAME,Int64,int64_t); \
    IMPLEMENT_ANY_SETOP(_msc_inline_bug,OPNAME,UInt64,uint64_t);

#define IMPLEMENT_SETOP_NUMERIC(OPNAME) \
    IMPLEMENT_SETOP_INTEGER(OPNAME); \
    IMPLEMENT_ANY_SETOP(__forceinline,OPNAME,Float,float); \
    IMPLEMENT_ANY_SETOP(__forceinline,OPNAME,Double,double);

#define IMPLEMENT_SETOP_SCALAR(OPNAME) \
    IMPLEMENT_ANY_SETOP(__forceinline,OPNAME,Bool,bool); \
    IMPLEMENT_SETOP_NUMERIC(OPNAME);

#define REGISTER_SETOP_INTEGER(OPNAME) \
    REGISTER_SETOP(OPNAME,Int,int32_t); \
    REGISTER_SETOP(OPNAME,UInt,uint32_t); \
    REGISTER_SETOP(OPNAME,Int64,int64_t); \
    REGISTER_SETOP(OPNAME,UInt64,uint64_t);

#define REGISTER_SETOP_NUMERIC(OPNAME) \
    REGISTER_SETOP_INTEGER(OPNAME); \
    REGISTER_SETOP(OPNAME,Float,float); \
    REGISTER_SETOP(OPNAME,Double,double);

#define REGISTER_SETOP_SCALAR(OPNAME) \
    REGISTER_SETOP(OPNAME,Bool,bool); \
    REGISTER_SETOP_NUMERIC(OPNAME); 

#define IMPLEMENT_SETOP_VEC(OPNAME,CTYPE) \
    IMPLEMENT_ANY_SETOP(__forceinline,OPNAME,,CTYPE)

#define IMPLEMENT_SETOP_INTEGER_VEC(OPNAME) \
    IMPLEMENT_SETOP_VEC(OPNAME,int2 ); \
    IMPLEMENT_SETOP_VEC(OPNAME,uint2); \
    IMPLEMENT_SETOP_VEC(OPNAME,int3 ); \
    IMPLEMENT_SETOP_VEC(OPNAME,uint3); \
    IMPLEMENT_SETOP_VEC(OPNAME,int4 ); \
    IMPLEMENT_SETOP_VEC(OPNAME,uint4);

#define IMPLEMENT_SETOP_NUMERIC_VEC(OPNAME)  \
    IMPLEMENT_SETOP_INTEGER_VEC(OPNAME); \
    IMPLEMENT_SETOP_VEC(OPNAME,float2 ); \
    IMPLEMENT_SETOP_VEC(OPNAME,float3 ); \
    IMPLEMENT_SETOP_VEC(OPNAME,float4 );

#define REGISTER_SETOP_VEC(OPNAME,CTYPE) \
    REGISTER_SETOP(OPNAME,,CTYPE)

#define REGISTER_SETOP_INTEGER_VEC(OPNAME) \
    REGISTER_SETOP_VEC(OPNAME,int2 ); \
    REGISTER_SETOP_VEC(OPNAME,uint2); \
    REGISTER_SETOP_VEC(OPNAME,int3 ); \
    REGISTER_SETOP_VEC(OPNAME,uint3); \
    REGISTER_SETOP_VEC(OPNAME,int4 ); \
    REGISTER_SETOP_VEC(OPNAME,uint4);

#define REGISTER_SETOP_NUMERIC_VEC(OPNAME) \
    REGISTER_SETOP_INTEGER_VEC(OPNAME); \
    REGISTER_SETOP_VEC(OPNAME,float2 ); \
    REGISTER_SETOP_VEC(OPNAME,float3 ); \
    REGISTER_SETOP_VEC(OPNAME,float4 );
