//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100




static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
       "강의",
       "식당",
       "실험실",
       "집",
       "실험실로 이동",
       "음식찬스",
       "축제시간"
};

static char smmNodeFood[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
       "치킨",
       "맥주",
       "탕후루",
       "컵라면",
       "감자탕",
       "컵밥",
       "짜장면",
       "학식",
       "파스타",
       "피자 ",
       "햄버거",
       "샌드위치",
       "요거트",
       "비빔밥"
};

char* smmObj_getTypeName(int type)
{
      return (char*)smmNodeName [type ];
}
/*
typedef enum smmObjGrade {
    smmObjGrade_Ap = 0,
    smmObjGrade_A0,
    smmObjGrade_Am,
    smmObjGrade_Bp,
    smmObjGrade_B0,
    smmObjGrade_Bm,
    smmObjGrade_Cp,
    smmObjGrade_C0,
    smmObjGrade_Cm
} smmObjGrade_e;
*/

//1. 구조체 형식 정의 
typedef struct smmObject {
       char name[MAX_CHARNAME];
       smmObjType_e objType; 
       int type;
       int credit;
       int energy;
       smmObjGrade_e grade;
} smmObject_t;

//음식 - 에너지 구조체 
typedef struct smmFood {
       char name[MAX_CHARNAME];
       int energy;
} smmFood_d;

//2.  구조체 배열 변수 정의   
//static smmObject_t smm_node[MAX_NODE];

//static int smmObj_noNode = 0;

#if 0
static char smmObj_name[MAX_NODE][MAX_CHARNAME];
static int smmObj_type[MAX_NODE];
static int smmObj_credit[MAX_NODE];
static int smmObj_energy[MAX_NODE];
#endif



//3. 관련 함수 변경 
//object generation
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{    
    smmObject_t* ptr;
    
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
}

//음식 생성 
void smmObj_foodCard(char* food_name, int food_energy)
{
	smmFood_d* foodptr;
	
	foodptr = (smmFood_d*)malloc(sizeof(smmFood_d));
	
	strcpy(Foodptr->food_name, food_name);
	foodptr->food_energy = food_energy;
	
	return foodptr;
}

    
    
    
/*
char* smmObj_getNodeName(void* obj)
    {
          smmObject_t*ptr = smmObject_t*)obj;
          
          return ptr -> name;
    }
          
    
    
    strcpy(smm_node[smmObj_noNode].name,name);
    smm_node[smmObj_noNode].type = type;
    smm_node[smmObj_noNode].credit = credit;
    smm_node[smmObj_noNode].energy = energy;
    
    smmObj_noNode++;
}
*/

//3. 관련 함수 변경 
char* smmObj_getNodeName(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}



//3. 관련 함수 변경 
int smmObj_getNodeType(void* obj)
{
    return smm_node[node_nr].type;
}

int smmObj_getNodeCredit(void* obj)
{
    return smm_node[node_nr].credit;
}

int smmObj_getNodeEnergy(void* obj)
{
    return smm_node[node_nr].energy;
}



/*
//member retrieving



//element to string
char* smmObj_getNodeName(smmNode_e type)
{
    return smmNodeName[type];
}

char* smmObj_getGradeName(smmGrade_e grade)
{
    return smmGradeName[grade];
}

*/
