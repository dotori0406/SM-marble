//
//  main.c
//  SMMarble 이세현  
//
//  Created by Juyeop Kim on 2023/11/05.
//
//2315397

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"


#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

#define Home_Energy        18


//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;
 

static int player_nr;

typedef struct player {
        int energy;
        int position;
        char name[MAX_CHARNAME];
        int accumCredit;
        int flag_graduate;
        int flag_laboratory; 
        
} player_t;

typedef struct food {
        char name[MAX_CHARNAME];
        int energy;

} food_d;
        
static player_t *cur_player;
static food_d *cur_food;
//static player_t cur_player[MAX_PLAYER];

#if 0
static int player_energy [MAX_PLAYER];
static int player_position [MAX_PLAYER];
static char player_name [MAX_PLAYER][MAX_CHARNAME];
#endif


//function prototypes
#if 0
//int isGraduated(void); //check if any player is graduated
//void printGrades(int player); //print grade history of the player
//void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
//void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
//void printGrades(int player); //print all the grade history of the player
#endif





void printPlayerStatus(void)
{
     int i ;
     for (i=0;i<player_nr;i++)
     {
         printf("%s : credit %i, energy %i, position %i\n",
                      cur_player[i].name,
                      cur_player[i].accumCredit,
                      cur_player[i].energy,
                      cur_player[i].position);
     }
 }

void generatePlayers(int n, int initEnergy) //generate a new player
{
     int i;
     //n time loop
     
     for (i=0;i<n;i++)
     {
         printf("Input player %i's name:", i); //안내문구
         scanf("%s", cur_player[i].name);
         fflush(stdin);
         
         
         //set position
         //player_position[i] = 0;
         cur_player[i].position = 0;
         
         //set energy
         //player_energy[i] = initEnergy;
         cur_player[i].energy = initEnergy;
         cur_player[i].accumCredit = 0;
         cur_player[i].flag_graduate = 0;
         cur_player[i].flag_laboratory = 0;
         //초기화 코드 
     }
}

     



//성적을 랜덤으로 1개 뽑는 함수 
char* getRandomGrade() {
  
    char* smmObjGrade_e[] = {
        "A+", "A0", "A-", "B+", "B0", "B-", "C+", "C0", "C-"
    };

    return smmObjGrade_e[rand() % (9)];
}

//랜덤하게 음식을 뽑는 함수 
char* getRandomFood() {
  
    char* smmFood_d[] = {
          "치킨", "맥주","탕후루", "컵라면","감자탕", "컵밥", "짜장면", "학식", "파스타", "피자", "햄버거", "샌드위치", "요거트", "비빔밥"

    };

    return smmFood_d[rand() %14];
}

//음식에 에너지를 랜덤하게 지정  
void* getRandomFoodEnergy(void) {
     int* foodenergy;
     foodenergy = rand()%16;

     }
     
//축제 미션을 랜덤으로 뽑는 함수 
char* getRandomFestival(){
      
      char* smmMission[] = {
            "노래한곡하고가시죠.", "졸업후포부한마디해주세요.", "오늘집에가서뭐하고싶어요?", "프로그래밍수업에대한소감한마디해주세요.", "동네맛집하나소개해주세요."
            };
            
      return  smmMission[rand()%5];
      
}
     

//성적 출력 
void printGrades(int player) 
{
     int i;
     void *gradePtr;
     char* smmGrade = getRandomGrade();
     for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
         printf("%s : %i\n", smmObj_getNodeName(gradePtr), smmObj_getNodeGrade(gradePtr));
     }
}

int rolldie(int player) //랜덤으로 주사위 값을 주는 함수  
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
#if 1 // 주사위를 굴릴 때  'g'를 누르면 상태를 확인할 수 있게 함  
    if (c == 'g')
        printGrades(player);
#endif
    
    return (rand()%MAX_DIE + 1);
}

int isGraduated(void) //졸업 학점을 채우는 지 확인  
{
    int i;
    for (i = 0; i < player_nr; i++)
    {
        if (cur_player[i].accumCredit >= GRADUATE_CREDIT)
        {
            cur_player[i].flag_graduate = 1
            break;
        }
    }
    
}

int lab_rolldie(void) //랜덤으로 주사위 값을 주는 함수 => 실험실용 
{
    char c;
    printf(" Press any key to roll a die ");
    c = getchar();
    
    return (rand() % 6) + 1;
}
      

int game_end(void) // 게임을 끝낼 때 사용  
{
    int i;
    int Game_end_flag;
    
    for (i = 0; i < player_nr; i++)
    {
        if (cur_player[i].flag_graduate == 1 && cur_player[i].position == SMMNODE_TYPE_HOME)
        {
            Game_end_flag = 1;
        }
    }
    
}
    
    


//action code when a player stays at a node
void actionNode(int player)
{
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
    //int type = smmObj_getNodeType( cur_player[player].position );
    int type = smmObj_getNodeType(boardPtr );
    char *name = smmObj_getNodeName(boardPtr );
    void *gradePtr;
    
    switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
            cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr );
            cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr );
            
            //grade generation
            char* smmGrade = getRandomGrade();
            gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit( boardPtr ), 0, smmGrade); 
            smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
            
        break;
            
        //식당칸
        case SMMNODE_TYPE_RESTAURANT:
             printf("식당에 들어왔습니다. 음식을 먹습니다.\n");
                 //음식을 랜덤으로 뽑아서 먹음 
                 char* smmFood_d = getRandomFood();
                 int* foodenergy = getRandomFoodEnergy();
                 printf("%s을(를) 먹습니다.\n", smmFood_d);
                 printf("%d 에너지가 회복되었습니다.\n", foodenergy);
                 printf("현재 에너지 총량은 %i 입니다. \n", cur_player[player].energy + foodenergy);
                 
        break;
        
        case SMMNODE_TYPE_LABORATORY : 
             printf("실험실에 들어왔습니다.\n");
             
             if (cur_player[i].flag_laboratory = 1) // 실험중일 때  flag_laboratory로 실험 중임을 구분  
             {
                 printf("실험 시간! 자 멋진 실험결과로 교수님을 만족시켜 봅시다. (3점 이상의 실험을 보여주자!)");
                 int labdice_result = lab_rolldie();
                 printf("당신의 실험 결과는 .. . . . .  %i 점! \n",labdice_result);
                 
                 if ( labdice_result >= 4 )
                 {
                      cur_player[player].flag_laboratory = 0;
                      printf("실험 성공!! 실험실을 벗어나 다음으로 갑시다!\n");
                 }
                 else  
                      printf(" 이런 결과가 좋지 않네요... 더 열심히 다시 도전해봅시다! \n");
             }
        
             else //실험 노드를 통해 실험실 칸을 온 것이 아닐 때 출력   
                 printf("실험시간 아님. 실험실 미개방.\n");          
          break; 
             
             
             
         case SMMNODE_TYPE_HOME : 
              //집에 들어오면 기본 에너지를 제공  
              printf("집에 들어왔습니다. 몸이 편안하네요. %i 만큼 에너지를 회복합니다.\n",Home_Energy); 
              printf("%s 의 총 에너지 : %i \n" ,cur_player[player].name,cur_player[player].energy + Home_Energy);
              
         break; 
         
         case SMMNODE_TYPE_GOTOLAB : 
              //이 칸을 통해서 실험 진입 가능  
              printf(" 이런! 실험시간이 다 되었네요. 플레이어 %s 는(은) 실험실로 이동해 실험을 진행합니다.",cur_player[player].name);
              cur_player[i].position = SMMNODE_TYPE_LABORATORY; //실험실로 이동  
              
              cur_player[player].flag_laboratory = 1; //실험 중으로 변환  
              
         break;
         
         case SMMNODE_TYPE_FOODCHANCE : 
              printf(" 플레이어 %s 는(은) 음식 찬스를 얻었습니다! Congraturation! \n",cur_player[player].name);
              printf(" 아무 키나 눌러 카드를 확인하세요!");
              getchar(); 
              //음식 카드 뽑기  
              char* smmFood_d = getRandomFood();
              int* foodenergy = getRandomFoodEnergy();
              printf("%s을(를) 먹습니다.\n", smmFood_d);
              printf("%d 에너지가 회복되었습니다.\n", foodenergy);
              printf("현재 에너지 총량은 %i 입니다. \n", cur_player[player].energy + foodenergy);
              
         break; 
         
         case SMMNODE_TYPE_FESTIVAL : 
              printf("%s 는(은) 축제 부스에 도착했습니다! 미션을 확인하고 부스를 클리어해봅시다! \n", cur_player[player].name);
              getchar(); //미션 확인과 카드 뽑기용  
              char* smmMission = getRandomFestival(); //미션 카드 뽑기  
              printf("당신의 미션은 [%s] 입니다\n", smmMission); 
              getcahr(); //미션 수행하기위함  
              printf("미션을 클리어 하셨네요! 다음으로 넘어갑시다!\n"); 
              getcahr();
           
          break;
              
               
               
                  
              
            
        default:
            break;
    }
}


void goForward(int player, int step)
{
     if (flag_laboratory == 0) //실험 중이면 동작 X  
     {
        void *boardPtr;
        cur_player[player].position += step;
        boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
        
        
        printf("%s go to node %i (type:%s)\n",
                cur_player[player].name,cur_player[player].position,
                smmObj_getNodeName(cur_player[player].position));
                
     }
     else //실험 중일 때 출력   
        printf("이런 플레이어 %s 는 현재 실험실에 있네요...\n",cur_player[player].name); 
 }



 

int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn = 0;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //read a node parameter set
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy,0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        if (type == SMMNODE_TYPE_HOME)
           initEnergy = energy;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    
    for (i = 0;i<board_nr;i++)
    {
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                     i, smmObj_getNodeName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
    }
    
    //printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    
    
    
    
  
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while () //read a food parameter set
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    
    
    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while () //read a festival card string
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
   
    

    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("input player no.:");
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player_nr >  MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    generatePlayers(player_nr, initEnergy);
    
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (Game_end_flag == 1) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)   
        die_result = rolldie(turn);
             
        
        //4-3. go forward
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn +1)%player_nr;
        
    }
    
    
    free(cur_player);
    system("PAUSE");
    return 0;
}
