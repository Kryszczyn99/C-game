#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <ncurses.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>

pthread_mutex_t mut;
sem_t semafor_bestia;
sem_t *semafor_pobran;
sem_t *semafor_koniec_pobran;
sem_t *sem1;
sem_t *sem2;
sem_t *semafor_koniec_programu;
sem_t *semafor_program;

struct zywot
{
    int wystartowal;
    int dzwignia;
};

struct zwloki
{
    int aktywny;
    int x;
    int y;
    int hajs;
};

struct send
{
    char tablica[5][5];
    int x,y;
    char ruch;
    char player_number;
    int rounds;
    int deaths;
    int type;
    int gold_carry;
    int bank;
    int bush;
    int servers_id;
    int graczy;
    int id;
    char last_sign;
    int zabity;
    int clear;
};


struct zywot *sprawdz1=NULL;
struct zywot *sprawdz2=NULL;
struct zywot *sprawdz3=NULL;
struct zywot *sprawdz4=NULL;
struct send *zmienna1=NULL;
struct send *zmienna2=NULL;
struct send *zmienna3=NULL;
struct send *zmienna4=NULL;
int *informacja=NULL;
int *info=NULL;


struct zwloki ciala[20];
int runda=0;
int ilosc_graczy=0;
int gracze[4]={0,0,0,0};
int tab_x=27,tab_y=53,camp_x=0,camp_y=0; 
int bestie=0;

char basic_tab[27][53]=
    {    
         {"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"},
         {"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"},
         {"%%   %       %   #####       %         %           %%"},
         {"%% % %%% %%% %%%%%%%%%%% %%% % %%%%%%% %%% %%%%%   %%"},
         {"%% %   % % %           % % %   %     %     %   %   %%"},
         {"%% % % % % %%%###%%%%% % % %%%%% %%%%%%%%%%%%% %%% %%"},
         {"%% % %   %           % % %  #  %       %       % % %%"},
         {"%% % %%%%% %%% %%%%%%% % % % %%% %%% %%% %%% % % % %%"},
         {"%% %         % %       % % %     %   %   % % %     %%"},
         {"%% % %%%%%%% %%% %%%%%%% % %%% %%% %%% %%% % %%% % %%"},
         {"%% % %     %   %   %     %   %   %   %         % % %%"},
         {"%% % % %%% %%% %%% %%% %%% % %%% %%%%%%%%%%% % % % %%"},
         {"%% %   %       % %   %     % %   % %       % %   % %%"},
         {"%% % %%%%%%#%% % %%% %%% %%% %%% % % %%%%% % % %%% %%"},
         {"%% %     %   % %   %   %   %   %   % %     % % %   %%"},
         {"%% % % ##% %%% %%% %%% %%%%%%% %%% % %%% %%% % % %%%%"},
         {"%% % %## %    #  %   % %  ###  %   %   %     % % % %%"},
         {"%% % %# #%%%%%%% % % % % %%#%%%% %%%%% %%%%%%% % % %%"},
         {"%% % %       %   % % %   %     %   % %       ##%   %%"},
         {"%% %%%%%%%%% % %%% %%%%%%% %%%%%%% % %%%%% % ##%%% %%"},
         {"%% % ##    % %     %     %       %   %   % % ##  % %%"},
         {"%% % %%%%% % %%%%%%% % %%% %%%%% %%% % % %%%#%%%%% %%"},
         {"%%## %     %         %  ## % # %     % %   %###### %%"},
         {"%% %%% %%%%%%%%%%%%%%%%%%%%%#% %%%%%%% %%% %#    # %%"},
         {"%%   %                 ##### % #         %    ##   %%"},
         {"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"},
         {"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"}
         
    };

char tab[27][53]=
    {    
         {"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"},
         {"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"},
         {"%%   %       %   #####       %         %           %%"},
         {"%% % %%% %%% %%%%%%%%%%% %%% % %%%%%%% %%% %%%%%   %%"},
         {"%% %   % % %           % % %   %     %     %   %   %%"},
         {"%% % % % % %%%###%%%%% % % %%%%% %%%%%%%%%%%%% %%% %%"},
         {"%% % %   %           % % %  #  %       %       % % %%"},
         {"%% % %%%%% %%% %%%%%%% % % % %%% %%% %%% %%% % % % %%"},
         {"%% %         % %       % % %     %   %   % % %     %%"},
         {"%% % %%%%%%% %%% %%%%%%% % %%% %%% %%% %%% % %%% % %%"},
         {"%% % %     %   %   %     %   %   %   %         % % %%"},
         {"%% % % %%% %%% %%% %%% %%% % %%% %%%%%%%%%%% % % % %%"},
         {"%% %   %       % %   %     % %   % %       % %   % %%"},
         {"%% % %%%%%%#%% % %%% %%% %%% %%% % % %%%%% % % %%% %%"},
         {"%% %     %   % %   %   %   %   %   % %     % % %   %%"},
         {"%% % % ##% %%% %%% %%% %%%%%%% %%% % %%% %%% % % %%%%"},
         {"%% % %## %    #  %   % %  ###  %   %   %     % % % %%"},
         {"%% % %# #%%%%%%% % % % % %%#%%%% %%%%% %%%%%%% % % %%"},
         {"%% % %       %   % % %   %     %   % %       ##%   %%"},
         {"%% %%%%%%%%% % %%% %%%%%%% %%%%%%% % %%%%% % ##%%% %%"},
         {"%% % ##    % %     %     %       %   %   % % ##  % %%"},
         {"%% % %%%%% % %%%%%%% % %%% %%%%% %%% % % %%%#%%%%% %%"},
         {"%%## %     %         %  ## % # %     % %   %###### %%"},
         {"%% %%% %%%%%%%%%%%%%%%%%%%%%#% %%%%%%% %%% %#    # %%"},
         {"%%   %                 ##### % #         %    ##   %%"},
         {"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"},
         {"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"}
         
    };
    

void dodaj_c()
{
    while(1)
    {
        int x=rand()%tab_x;
        int y=rand()%tab_y;
        if(tab[x][y]!='%' && tab[x][y]!='#' && tab[x][y]!='c' && tab[x][y]!='t' && 
           tab[x][y]!='T' && tab[x][y]!='A' && tab[x][y]!='1' && tab[x][y]!='2' &&
           tab[x][y]!='3' && tab[x][y]!='4' && tab[x][y]!='*')
        {
            tab[x][y]='c';
            break;
        }
    }
}
void losuj_kordy(int *xx,int *yy)
{
    while(1)
    {
        int x=rand()%tab_x;
        int y=rand()%tab_y;
        if(tab[x][y]!='%' && tab[x][y]!='#' && tab[x][y]!='c' && tab[x][y]!='t' && 
           tab[x][y]!='T' && tab[x][y]!='A' && tab[x][y]!='1' && tab[x][y]!='2' &&
           tab[x][y]!='3' && tab[x][y]!='4' && tab[x][y]!='*')
            {
                *xx=x;
                *yy=y;
                break;
            }
    }
}
void dodaj_t()
{
    while(1)
    {
        int x=rand()%tab_x;
        int y=rand()%tab_y;
        if(tab[x][y]!='%' && tab[x][y]!='#' && tab[x][y]!='c' && tab[x][y]!='t' && 
           tab[x][y]!='T' && tab[x][y]!='A' && tab[x][y]!='1' && tab[x][y]!='2' &&
           tab[x][y]!='3' && tab[x][y]!='4' && tab[x][y]!='*')
        {
            tab[x][y]='t';
            break;
        }
    }
}

void dodaj_T()
{
    while(1)
    {
        int x=rand()%tab_x;
        int y=rand()%tab_y;
        if(tab[x][y]!='%' && tab[x][y]!='#' && tab[x][y]!='c' && tab[x][y]!='t' && 
           tab[x][y]!='T' && tab[x][y]!='A' && tab[x][y]!='1' && tab[x][y]!='2' &&
           tab[x][y]!='3' && tab[x][y]!='4' && tab[x][y]!='*')
        {
            tab[x][y]='T';
            break;
        }
    }
}

void narysuj()
{
    mvprintw(3,55,"Server's PID: %d",getpid());
    mvprintw(4,56,"Campsite X/Y: %d/%d",camp_x,camp_y);
    mvprintw(5,56,"Round numer: 0");
    mvprintw(7,55,"Parameter:   Player1  Player2  Player3  Player4");
    mvprintw(8,55," PID         -        -        -        -");
    mvprintw(9,55," Type        -        -        -        -");
    mvprintw(10,55," Curr X/Y    --/--    --/--    --/--    --/--");
    mvprintw(11,55," Deaths      -        -        -        -");
    mvprintw(13,55," Coins");
    mvprintw(14,55,"     carried 0000     0000     0000     0000");
    mvprintw(15,55,"     brought 0000     0000     0000     0000");
    mvprintw(18,55,"Legend:");
    attron(COLOR_PAIR(6));
    mvprintw(19,56,"1234");
    attroff(COLOR_PAIR(6));
    mvprintw(19,60," - players");
    attron(COLOR_PAIR(3));
    mvprintw(20,56,"%    - wall");
    attroff(COLOR_PAIR(3));
    mvprintw(20,57,"    - wall (black rectangle)");
    mvprintw(21,56,"#    - bushes (slow down)");
    attron(COLOR_PAIR(7));
    mvprintw(22,56,"*    - wild beast");
    attroff(COLOR_PAIR(7));
    mvprintw(22,57,"    - wild beast");
    attron(COLOR_PAIR(1));
    mvprintw(23,56,"c    - one coin");
    attroff(COLOR_PAIR(1));
    mvprintw(23,57,"    - one coin");
    attron(COLOR_PAIR(1));
    mvprintw(24,56,"t    - treasure (10coins)");
    attroff(COLOR_PAIR(1));
    mvprintw(24,57,"    - treasure (10coins)");
    attron(COLOR_PAIR(1));
    mvprintw(25,56,"T    - large treasure (50coins)");
    attroff(COLOR_PAIR(1));
    mvprintw(25,57,"    - large treasure (50coins)");
    attron(COLOR_PAIR(2));
    mvprintw(26,56,"A    - campsite");
    attroff(COLOR_PAIR(2));
    mvprintw(26,57,"    - campsite");
    attron(COLOR_PAIR(2));
    mvprintw(27,56,"D    - dropped treasure");
    attroff(COLOR_PAIR(2));
    mvprintw(27,57,"    - dropped treasure");
    refresh();
}

int czy_znak(char znak)
{
    if(znak=='1') return 1;
    if(znak=='2') return 1;
    if(znak=='3') return 1;
    if(znak=='4') return 1;
    return 0;
}

void respawn(struct send* zmienna)
{
                pthread_mutex_lock(&mut);
                zmienna->zabity=1;
                zmienna->clear=1;
                if(zmienna->gold_carry==0)
                {
                    int m_x=zmienna->x,m_y=zmienna->y;
                    if(basic_tab[m_x][m_y]=='#') tab[m_x][m_y]='#';
                    else tab[m_x][m_y]=' ';
                    tab[camp_x][camp_y]='A';
                    int l,f;
                    losuj_kordy(&l,&f);
                    zmienna->x=l;
                    zmienna->y=f;
                    zmienna->last_sign=' ';
                    zmienna->deaths+=1;
                }
                else
                {
                    int q=0;
                    for(q=0;q<20;q++)
                    {
                        if(ciala[q].aktywny==0) break;
                    }
                    ciala[q].hajs=zmienna->gold_carry;
                    zmienna->gold_carry=0;
                    tab[zmienna->x][zmienna->y]='D';
                    ciala[q].x=zmienna->x;
                    ciala[q].y=zmienna->y;
                    ciala[q].aktywny=1;
                    int l,f;
                    losuj_kordy(&l,&f);
                    zmienna->x=l;
                    zmienna->y=f;
                    zmienna->last_sign=' ';
                    zmienna->deaths+=1;
                }                
                pthread_mutex_unlock(&mut);
}

void *bestia(void *par)
{
    int x,y;
    losuj_kordy(&x,&y);
    char znak=tab[x][y];
    tab[x][y]='*';
    char temp[5][5];
    pthread_mutex_lock(&mut);
    int p=-2;
    for(int i=0;i<5;i++,p++)
    {
        for(int j=0,g=-2;j<5;j++,g++) temp[i][j]=tab[x+p][y+g];
    }
    pthread_mutex_unlock(&mut);
    while(1)
    {
                    int flaga=0;
                    int fl=0;
                    p=-2;
                    for(int i=0;i<5;i++)
                    {
                        for(int j=0;j<5;j++)
                        {
                            char z=temp[i][j];
                            if(z=='1' || z=='2' || z=='3' || z=='4') flaga=1;
                        }
                    }
                    if(flaga==1)                
                    {
                        if(czy_znak(temp[1][1]))
                        {
                            if(temp[1][2]!='%')
                            {
                                pthread_mutex_lock(&mut);
                                tab[x][y]=znak;
                                znak=tab[x-1][y];
                                if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                                {
                                    if(basic_tab[x][y]=='#') znak='#';
                                    else znak=' ';
                                }
                                tab[x-1][y]='*';
                                pthread_mutex_unlock(&mut);
                                x-=1;
                                fl=1;
                            }
                            else if(temp[2][1]!='%')
                            {
                                pthread_mutex_lock(&mut);
                                tab[x][y]=znak;
                                znak=tab[x][y-1];
                                if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                                {
                                    if(basic_tab[x][y]=='#') znak='#';
                                    else znak=' ';
                                }
                                tab[x][y-1]='*';
                                pthread_mutex_unlock(&mut);
                                y-=1;
                                fl=1;
                            }
                        }
                        else if(czy_znak(temp[1][2]))
                        {
                            char zn=temp[1][2];
                            if(zn=='1') respawn(zmienna1);
                            else if(zn=='2') respawn(zmienna2);
                            else if(zn=='3') respawn(zmienna3);
                            else if(zn=='4') respawn(zmienna4);
                            fl=1;
                        }
                        else if(czy_znak(temp[1][3]))
                        {
                            if(temp[1][2]!='%')
                            {
                                pthread_mutex_lock(&mut);
                                tab[x][y]=znak;
                                znak=tab[x-1][y];
                                if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                                {
                                    if(basic_tab[x][y]=='#') znak='#';
                                    else znak=' ';
                                }
                                tab[x-1][y]='*';
                                pthread_mutex_unlock(&mut);
                                x-=1;
                                fl=1;
                            }
                            else if(temp[2][3]!='%')
                            {
                                pthread_mutex_lock(&mut);
                                tab[x][y]=znak;
                                znak=tab[x][y+1];
                                if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                                {
                                    if(basic_tab[x][y]=='#') znak='#';
                                    else znak=' ';
                                }
                                tab[x][y+1]='*';
                                pthread_mutex_unlock(&mut);
                                y+=1;
                                fl=1;
                            }
                        }
                        else if(czy_znak(temp[2][1]))
                        {
                            char zn=temp[2][1];
                            if(zn=='1') respawn(zmienna1);
                            else if(zn=='2') respawn(zmienna2);
                            else if(zn=='3') respawn(zmienna3);
                            else if(zn=='4') respawn(zmienna4);
                            fl=1;
                        }
                        else if(czy_znak(temp[2][3]))
                        {
                            char zn=temp[2][3];
                            if(zn=='1') respawn(zmienna1);
                            else if(zn=='2') respawn(zmienna2);
                            else if(zn=='3') respawn(zmienna3);
                            else if(zn=='4') respawn(zmienna4);
                            fl=1;
                        }
                        else if(czy_znak(temp[3][1]))
                        {
                            if(temp[2][1]!='%')
                            {
                                pthread_mutex_lock(&mut);
                                tab[x][y]=znak;
                                znak=tab[x][y-1];
                                if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                                {
                                    if(basic_tab[x][y]=='#') znak='#';
                                    else znak=' ';
                                }
                                tab[x][y-1]='*';
                                pthread_mutex_unlock(&mut);
                                y-=1;
                                fl=1;
                            }
                            else if(temp[3][2]!='%')
                            {
                                pthread_mutex_lock(&mut);
                                tab[x][y]=znak;
                                znak=tab[x+1][y];
                                if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                                {
                                    if(basic_tab[x][y]=='#') znak='#';
                                    else znak=' ';
                                }
                                tab[x+1][y]='*';
                                pthread_mutex_unlock(&mut);
                                x+=1;
                                fl=1;
                            }
                        }
                        else if(czy_znak(temp[3][2]))
                        {
                            char zn=temp[3][2];
                            if(zn=='1') respawn(zmienna1);
                            else if(zn=='2') respawn(zmienna2);
                            else if(zn=='3') respawn(zmienna3);
                            else if(zn=='4') respawn(zmienna4);
                            fl=1;
                        }
                        else if(czy_znak(temp[3][3]))
                        {
                            if(temp[2][3]!='%')
                            {
                                pthread_mutex_lock(&mut);
                                tab[x][y]=znak;
                                znak=tab[x][y+1];
                                if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                                {
                                    if(basic_tab[x][y]=='#') znak='#';
                                    else znak=' ';
                                }
                                tab[x][y+1]='*';
                                pthread_mutex_unlock(&mut);
                                y+=1;
                                fl=1;
                            }
                            else if(temp[3][2]!='%')
                            {
                                pthread_mutex_lock(&mut);
                                tab[x][y]=znak;
                                znak=tab[x+1][y];
                                if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                                {
                                    if(basic_tab[x][y]=='#') znak='#';
                                    else znak=' ';
                                }
                                tab[x+1][y]='*';
                                pthread_mutex_unlock(&mut);
                                x+=1;
                                fl=1;
                            }
                        }
                        else if(czy_znak(temp[0][2]) && temp[1][2]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x-1][y];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x-1][y]='*';
                            pthread_mutex_unlock(&mut);
                            x-=1;
                            fl=1;
                        }
                        else if(czy_znak(temp[4][2]) && temp[3][2]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x+1][y];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x+1][y]='*';
                            pthread_mutex_unlock(&mut);
                            x+=1;
                            fl=1;
                        }
                        else if(czy_znak(temp[2][0]) && temp[2][1]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x][y-1];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x][y-1]='*';
                            pthread_mutex_unlock(&mut);
                            y-=1;
                            fl=1;
                        }
                        else if(czy_znak(temp[2][4]) && temp[2][3]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x][y+1];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x][y+1]='*';
                            pthread_mutex_unlock(&mut);
                            y+=1;
                            fl=1;
                        }
                        else if(czy_znak(temp[0][1]) && temp[1][2]!='%' && temp[1][1]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x-1][y];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x-1][y]='*';
                            pthread_mutex_unlock(&mut);
                            x-=1;
                            fl=1;
                        }
                        else if(czy_znak(temp[0][3]) && temp[1][2]!='%' && temp[1][3]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x-1][y];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x-1][y]='*';
                            pthread_mutex_unlock(&mut);
                            x-=1;
                            fl=1;
                        }
                        else if(czy_znak(temp[1][0]) && temp[2][1]!='%' && temp[1][1]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x][y-1];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x][y-1]='*';
                            pthread_mutex_unlock(&mut);
                            y-=1;
                            fl=1;
                        }
                        else if(czy_znak(temp[3][0]) && temp[2][1]!='%' && temp[3][1]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x][y-1];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x][y-1]='*';
                            pthread_mutex_unlock(&mut);
                            y-=1;
                            fl=1;
                        }
                        else if(czy_znak(temp[4][1]) && temp[3][2]!='%' && temp[3][1]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x+1][y];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x+1][y]='*';
                            pthread_mutex_unlock(&mut);
                            x+=1;
                            fl=1;
                        }
                        else if(czy_znak(temp[4][3]) && temp[3][2]!='%' && temp[3][3]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x+1][y];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x+1][y]='*';
                            pthread_mutex_unlock(&mut);
                            x+=1;
                            fl=1;
                        }
                        else if(czy_znak(temp[1][4]) && temp[2][3]!='%' && temp[1][3]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x][y+1];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x][y+1]='*';
                            pthread_mutex_unlock(&mut);
                            y+=1;
                            fl=1;
                        }
                        else if(czy_znak(temp[3][4]) && temp[2][3]!='%' && temp[3][3]!='%')
                        {                            
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x][y+1];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x][y+1]='*';
                            pthread_mutex_unlock(&mut);
                            y+=1;
                            fl=1;
                        }
                    }
                    if(fl==0)
                    {
                        int los=rand()%4;
                        if(los==0 && tab[x+1][y]!='%')
                        {
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x+1][y];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x+1][y]='*';
                            pthread_mutex_unlock(&mut);
                            x+=1;
                        }
                        else if(los==1 && tab[x][y+1]!='%')
                        {
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x][y+1];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x][y+1]='*';
                            pthread_mutex_unlock(&mut);
                            y+=1;
                        }
                        else if(los==2 && tab[x-1][y]!='%')
                        {
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x-1][y];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x-1][y]='*';
                            pthread_mutex_unlock(&mut);
                            x-=1;
                        }
                        else if(los==3 && tab[x][y-1]!='%')
                        {
                            pthread_mutex_lock(&mut);
                            tab[x][y]=znak;
                            znak=tab[x][y-1];
                            if(znak=='1' || znak=='2' || znak=='3' || znak=='4' || znak=='*')
                            {
                                if(basic_tab[x][y]=='#') znak='#';
                                else znak=' ';
                            }
                            tab[x][y-1]='*';
                            pthread_mutex_unlock(&mut);
                            y-=1;
                        }
                    }
                    pthread_mutex_lock(&mut);
                    p=-2;
                    for(int i=0;i<5;i++,p++)
                    {
                        for(int j=0,g=-2;j<5;j++,g++)
                        {
                            temp[i][j]=tab[x+p][y+g];
                        }
                    } 
                    pthread_mutex_unlock(&mut);
                
            refresh();
            usleep(1000000);
            
    }
    
    return NULL;
}

void dodajemy_zmienna()
{
   //dodawanie monet c (1punkt)
    
    int p=0;
    while(p<=10)
    {
        int x=rand()%tab_x;
        int y=rand()%tab_y;
        if(tab[x][y]!='%' && tab[x][y]!='#')
        {
            tab[x][y]='c';
            p++;
        }
    }
    
    //dodawanie skarbu t (10punktow)
    
    p=0;
    while(p<=4)
    {
        int x=rand()%tab_x;
        int y=rand()%tab_y;
        if(tab[x][y]!='%' && tab[x][y]!='#' && tab[x][y]!='c')
        {
            tab[x][y]='t';
            p++;
        }
    }
    
    //dodawanie skarbu T (50pkt)
    
    p=0;
    while(p<=1)
    {
        int x=rand()%tab_x;
        int y=rand()%tab_y;
        if(tab[x][y]!='%' && tab[x][y]!='#' && tab[x][y]!='c' && tab[x][y]!='C')
        {
            tab[x][y]='T';
            p++;
        }
    }
    
    //dodanie kampusu
    
    p=0;
    while(p<=0)
    {
        int x=rand()%tab_x;
        int y=rand()%tab_y;
        if(tab[x][y]!='%' && tab[x][y]!='#' && tab[x][y]!='c' && tab[x][y]!='C' && tab[x][y]!='T')
        {
            tab[x][y]='A';
            p++;
            camp_x=x;
            camp_y=y;
        }
    }
    pthread_t beast;
    pthread_create(&beast,NULL,bestia,NULL);
    bestie+=1;
}

void *drukowanie(void* par)
{
    while(1)
    {
        pthread_mutex_lock(&mut);
        int q=0;
        for(q=0;q<20;q++)
        {
            if(ciala[q].aktywny==1)
            {
                tab[ciala[q].x][ciala[q].y]='D';
            }
        }
        if(ilosc_graczy==0)
        {
            mvprintw(5,56,"Round numer: 0     ");
            runda=0;
        } 
        else mvprintw(5,56,"Round numer: %d",runda);
        if(gracze[0]==0)
        {
            mvprintw(8,68,"-    ");
            mvprintw(9,68,"-    ");
            mvprintw(10,68,"--/--");
            mvprintw(11,68,"-    ");
            mvprintw(14,68,"0000");
            mvprintw(15,68,"0000");
        }
        else
        {
            mvprintw(8,68,"%d",zmienna1->id);
            if(zmienna1->type==0) mvprintw(9,68,"BOT");
            else mvprintw(9,68,"HUMAN");
            mvprintw(10,68,"%02d/%02d",zmienna1->x,zmienna1->y);
            mvprintw(11,68,"%d",zmienna1->deaths);
            mvprintw(14,68,"%04d",zmienna1->gold_carry);
            mvprintw(15,68,"%04d",zmienna1->bank);
        }
        if(gracze[1]==0)
        {
            mvprintw(8,77,"-    ");
            mvprintw(9,77,"-    ");
            mvprintw(10,77,"--/--");
            mvprintw(11,77,"-    ");
            mvprintw(14,77,"0000");
            mvprintw(15,77,"0000");
        }
        else
        {
            mvprintw(8,77,"%d",zmienna2->id);
            if(zmienna2->type==0) mvprintw(9,77,"BOT");
            else mvprintw(9,77,"HUMAN");
            mvprintw(10,77,"%02d/%02d",zmienna2->x,zmienna2->y);
            mvprintw(11,77,"%d",zmienna2->deaths);
            mvprintw(14,77,"%04d",zmienna2->gold_carry);
            mvprintw(15,77,"%04d",zmienna2->bank);
        }
        if(gracze[2]==0)
        {
            mvprintw(8,86,"-    ");
            mvprintw(9,86,"-    ");
            mvprintw(10,86,"--/--");
            mvprintw(11,86,"-    ");
            mvprintw(14,86,"0000");
            mvprintw(15,86,"0000");
        }
        else
        {
            mvprintw(8,86,"%d",zmienna3->id);
            if(zmienna3->type==0) mvprintw(9,86,"BOT");
            else mvprintw(9,86,"HUMAN");
            mvprintw(10,86,"%02d/%02d",zmienna3->x,zmienna3->y);
            mvprintw(11,86,"%d",zmienna3->deaths);
            mvprintw(14,86,"%04d",zmienna3->gold_carry);
            mvprintw(15,86,"%04d",zmienna3->bank);
        }
        if(gracze[3]==0)
        {
            mvprintw(8,95,"-    ");
            mvprintw(9,95,"-    ");
            mvprintw(10,95,"--/--");
            mvprintw(11,95,"-    ");
            mvprintw(14,95,"0000");
            mvprintw(15,95,"0000");
        }
        else
        {
            mvprintw(8,95,"%d",zmienna4->id);
            if(zmienna4->type==0) mvprintw(9,95,"BOT");
            else mvprintw(9,95,"HUMAN");
            mvprintw(10,95,"%02d/%02d",zmienna4->x,zmienna4->y);
            mvprintw(11,95,"%d",zmienna4->deaths);
            mvprintw(14,95,"%04d",zmienna4->gold_carry);
            mvprintw(15,95,"%04d",zmienna4->bank);           
        }    
        refresh();
        for(int i=0;i<tab_x;i++)
        {
            for(int j=0;j<tab_y;j++)
            {
                if((tab[i][j])=='c' || (tab[i][j])=='t' || (tab[i][j])=='T' )
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%c",tab[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(1));
                }
                else if((tab[i][j])=='A')
                {
                    attron(COLOR_PAIR(2));
                    mvprintw(i,j,"%c",tab[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(2));                   
                }
                else if((tab[i][j])=='%')
                {
                    attron(COLOR_PAIR(8));
                    mvprintw(i,j,"%c",tab[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(8));                   
                }
                else if((tab[i][j])==' ')
                {
                    attron(COLOR_PAIR(4));
                    mvprintw(i,j,"%c",tab[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(4));                   
                }
                else if((tab[i][j])=='#')
                {
                    attron(COLOR_PAIR(5));
                    mvprintw(i,j,"%c",tab[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(5));                   
                }
                else if((tab[i][j])=='1' || (tab[i][j])=='2' || (tab[i][j])=='3' || (tab[i][j])=='4')
                {
                    attron(COLOR_PAIR(6));
                    mvprintw(i,j,"%c",tab[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(6));                   
                }
                else if((tab[i][j])=='*')
                {
                    attron(COLOR_PAIR(7));
                    mvprintw(i,j,"%c",tab[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(7));                   
                }
                else if((tab[i][j])=='D')
                {
                    attron(COLOR_PAIR(2));
                    mvprintw(i,j,"%c",tab[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(2));                       
                }
                else
                {
                    attron(COLOR_PAIR(3));
                    mvprintw(i,j," ");
                    refresh();
                    attroff(COLOR_PAIR(3));
                }
            }
        }
        pthread_mutex_unlock(&mut);
        usleep(50000);
    }
    return NULL;
}

void *klawiatura(void *par)
{
    while(1)
    {
        int znak=getchar();
        if(znak=='Q' || znak=='q') break;
        else if((znak=='B' || znak=='b') && bestie<=10)
        {
            pthread_t beastt;
            pthread_create(&beastt,NULL,bestia,NULL);
            bestie++;
        }
        else if(znak=='c') dodaj_c();
        else if(znak=='t') dodaj_t();
        else if(znak=='T') dodaj_T();
    }
    return NULL;
}
void init(struct send *zmienna, int s)
{
    zmienna->player_number='0'+1+s;
    zmienna->deaths=0;
    zmienna->type=0;
    zmienna->gold_carry=0;
    zmienna->bank=0;
    zmienna->bush=0;
    zmienna->servers_id=getpid();
    zmienna->rounds=runda;
    zmienna->graczy=ilosc_graczy+1;
    zmienna->last_sign=' ';
    int l,f;
    losuj_kordy(&l,&f);
    zmienna->x=l;
    zmienna->y=f;
    tab[l][f]='0'+1+s;
    int p=-2;
    for(int i=0;i<5;i++,p++)
    {
        int g=-2;
        for(int j=0;j<5;j++,g++)
        {
            zmienna->tablica[i][j]=tab[l+p][f+g];
        }
    }
}
void *przyjmowanie_klientow(void *par)
{
    while(1)
    {
        sem_wait(semafor_pobran);
        if(ilosc_graczy!=4)
        {
            int i;
            for(i=0;i<4;i++)
            {
                if(gracze[i]==0) break;
            }
            gracze[i]=1;
            *informacja=i+1;
            if(i==0) init(zmienna1,i);
            else if(i==1) init(zmienna2,i);
            else if(i==2) init(zmienna3,i);
            else if(i==3) init(zmienna4,i);
            ilosc_graczy+=1;
        }
        else *informacja=-1;
        sem_post(semafor_koniec_pobran);
    }
    return NULL;
}

void *kierowanie(void *par)
{
    char zn=' ';
    while(1)
    {
        sem_wait(sem2);
        struct send* zmienna=NULL;

        for(int i=0;i<4;i++)
        {
            if(i==0 && gracze[0]==0) continue;
            else if(i==1 && gracze[1]==0) continue;
            else if(i==2 && gracze[2]==0) continue;
            else if(i==3 && gracze[3]==0) continue;
            if(i==0)
            {
                zmienna=zmienna1;
                zn=zmienna1->last_sign;
            }
            else if(i==1)
            {
                zmienna=zmienna2;
                zn=zmienna2->last_sign;
            }
            else if(i==2)
            {
                zmienna=zmienna3;
                zn=zmienna3->last_sign;
            }
            else if(i==3)
            {
                zmienna=zmienna4;
                zn=zmienna4->last_sign;
            }
            if(zmienna->ruch=='A')
            {
                if(tab[zmienna->x-1][zmienna->y]=='#' && zmienna->bush==0)
                {
                    tab[zmienna->x][zmienna->y]=zn;
                    zn=tab[zmienna->x-1][zmienna->y];
                    zmienna->x-=1;
                    tab[zmienna->x][zmienna->y]=zmienna->player_number; 
                }
                else if(tab[zmienna->x-1][zmienna->y]!='%' && zmienna->bush==0)
                {
                    tab[zmienna->x][zmienna->y]=zn;
                    zn=tab[zmienna->x-1][zmienna->y];
                    zmienna->x-=1;
                    tab[zmienna->x][zmienna->y]=zmienna->player_number;
                }
            }
            else if(zmienna->ruch=='B')
            {
                if(tab[zmienna->x+1][zmienna->y]=='#' && zmienna->bush==0)
                {
                    tab[zmienna->x][zmienna->y]=zn;
                    zn=tab[zmienna->x+1][zmienna->y];
                    zmienna->x+=1;
                    tab[zmienna->x][zmienna->y]=zmienna->player_number;
                }
                else if(tab[zmienna->x+1][zmienna->y]!='%' && zmienna->bush==0)
                {
                    tab[zmienna->x][zmienna->y]=zn;
                    zn=tab[zmienna->x+1][zmienna->y];
                    zmienna->x+=1;
                    tab[zmienna->x][zmienna->y]=zmienna->player_number;
                }            
            }
            
            else if(zmienna->ruch=='C')
            {
                if(tab[zmienna->x][zmienna->y+1]=='#' && zmienna->bush==0)
                {
                    tab[zmienna->x][zmienna->y]=zn;
                    zn=tab[zmienna->x][zmienna->y+1];
                    zmienna->y+=1;
                    tab[zmienna->x][zmienna->y]=zmienna->player_number;
                }
                else if(tab[zmienna->x][zmienna->y+1]!='%' && zmienna->bush==0)
                {
                    tab[zmienna->x][zmienna->y]=zn;
                    zn=tab[zmienna->x][zmienna->y+1];
                    zmienna->y+=1;
                    tab[zmienna->x][zmienna->y]=zmienna->player_number;
                }            
            }
            else if(zmienna->ruch=='D')
            {
                if(tab[zmienna->x][zmienna->y-1]=='#' && zmienna->bush==0)
                {
                    tab[zmienna->x][zmienna->y]=zn;
                    zn=tab[zmienna->x][zmienna->y-1];
                    zmienna->y-=1;
                    tab[zmienna->x][zmienna->y]=zmienna->player_number;
                }
                else if(tab[zmienna->x][zmienna->y-1]!='%' && zmienna->bush==0)
                {
                    tab[zmienna->x][zmienna->y]=zn;
                    zn=tab[zmienna->x][zmienna->y-1];
                    zmienna->y-=1;
                    tab[zmienna->x][zmienna->y]=zmienna->player_number;
                }            
            }
            zmienna->ruch=' ';
            if(zn=='c')
            {
                zn=' ';
                zmienna->gold_carry+=1;
            }

            if(zn=='t')
            {
                zn=' ';    
                zmienna->gold_carry+=10;
            }
            if(zn=='T')
            {
                zn=' ';
                zmienna->gold_carry+=50;
            }
            if(zn=='D')
            {
                int flaga=0;
                int ite=0;
                for(;ite<20;ite++)
                {
                    if(ciala[ite].aktywny==1)
                    {
                        if(ciala[ite].x==zmienna->x && ciala[ite].y==zmienna->y)
                        {
                            flaga=1;
                            break;
                        }
                    }
                }
                if(flaga==1)
                {
                    zmienna->gold_carry+=ciala[ite].hajs;
                    if(basic_tab[ciala[ite].x][ciala[ite].y]=='#') zn='#';
                    else zn=' ';
                    ciala[ite].aktywny=0;
                }
            }
            if(zn=='A')
            {
                zmienna->bank+=zmienna->gold_carry;
                zmienna->gold_carry=0;
            }
            if(zn=='#')
            {
                zmienna->bush+=1;
                if(zmienna->bush==2)
                {
                    zmienna->bush=0;
                }
            }
            if(zn=='1')
            {
                if(zmienna->gold_carry==0 && zmienna1->gold_carry==0)
                {
                    int m_x=zmienna->x,m_y=zmienna->y;
                    if(basic_tab[m_x][m_y]=='#') tab[m_x][m_y]='#';
                    else tab[m_x][m_y]=' ';
                    tab[camp_x][camp_y]='A';
                    int l,f;
                    losuj_kordy(&l,&f);
                    zmienna->x=l;
                    zmienna->y=f;
                    losuj_kordy(&l,&f);
                    zmienna1->x=l;
                    zmienna1->y=l;
                    zmienna->last_sign=' ';
                    zmienna1->last_sign=' ';
                    zmienna->deaths+=1;
                    zmienna1->deaths+=1;
                    zn=' ';
                }
                else
                {
                    int q=0;
                    for(q=0;q<20;q++)
                    {
                        if(ciala[q].aktywny==0) break;
                    }
                    ciala[q].hajs=zmienna->gold_carry+zmienna1->gold_carry;
                    zmienna->gold_carry=0;
                    zmienna1->gold_carry=0;
                    tab[zmienna->x][zmienna->y]='D';
                    ciala[q].x=zmienna->x;
                    ciala[q].y=zmienna->y;
                    ciala[q].aktywny=1;
                    int l,f;
                    losuj_kordy(&l,&f);
                    zmienna->x=l;
                    zmienna->y=f;
                    losuj_kordy(&l,&f);
                    zmienna1->x=l;
                    zmienna1->y=l;
                    zmienna->last_sign=' ';
                    zmienna1->last_sign=' ';
                    zmienna->deaths+=1;
                    zmienna1->deaths+=1;
                    zn=' ';
                }
            }
            else if(zn=='2')
            {
                if(zmienna->gold_carry==0 && zmienna2->gold_carry==0)
                {
                    int m_x=zmienna->x,m_y=zmienna->y;
                    if(basic_tab[m_x][m_y]=='#') tab[m_x][m_y]='#';
                    else tab[m_x][m_y]=' ';
                    tab[camp_x][camp_y]='A';
                    int l,f;
                    losuj_kordy(&l,&f);
                    zmienna->x=l;
                    zmienna->y=f;
                    losuj_kordy(&l,&f);
                    zmienna2->x=l;
                    zmienna2->y=l;
                    zmienna->last_sign=' ';
                    zmienna2->last_sign=' ';
                    zmienna->deaths+=1;
                    zmienna2->deaths+=1;
                    zn=' ';
                }
                else
                {
                    int q=0;
                    for(q=0;q<20;q++)
                    {
                        if(ciala[q].aktywny==0) break;
                    }
                    ciala[q].hajs=zmienna->gold_carry+zmienna2->gold_carry;
                    zmienna->gold_carry=0;
                    zmienna2->gold_carry=0;
                    tab[zmienna->x][zmienna->y]='D';
                    ciala[q].x=zmienna->x;
                    ciala[q].y=zmienna->y;
                    ciala[q].aktywny=1;
                    int l,f;
                    losuj_kordy(&l,&f);
                    zmienna->x=l;
                    zmienna->y=f;
                    losuj_kordy(&l,&f);
                    zmienna2->x=l;
                    zmienna2->y=l;
                    zmienna->last_sign=' ';
                    zmienna2->last_sign=' ';
                    zmienna->deaths+=1;
                    zmienna2->deaths+=1;
                    zn=' ';
                }
            }
            else if(zn=='3')
            {
                if(zmienna->gold_carry==0 && zmienna3->gold_carry==0)
                {
                    int m_x=zmienna->x,m_y=zmienna->y;
                    if(basic_tab[m_x][m_y]=='#') tab[m_x][m_y]='#';
                    else tab[m_x][m_y]=' ';
                    tab[camp_x][camp_y]='A';
                    int l,f;
                    losuj_kordy(&l,&f);
                    zmienna->x=l;
                    zmienna->y=f;
                    losuj_kordy(&l,&f);
                    zmienna3->x=l;
                    zmienna3->y=l;
                    zmienna->last_sign=' ';
                    zmienna3->last_sign=' ';
                    zmienna->deaths+=1;
                    zmienna3->deaths+=1;
                    zn=' ';
                }
                else
                {
                    int q=0;
                    for(q=0;q<20;q++)
                    {
                        if(ciala[q].aktywny==0) break;
                    }
                    ciala[q].hajs=zmienna->gold_carry+zmienna3->gold_carry;
                    zmienna->gold_carry=0;
                    zmienna3->gold_carry=0;
                    tab[zmienna->x][zmienna->y]='D';
                    ciala[q].x=zmienna->x;
                    ciala[q].y=zmienna->y;
                    ciala[q].aktywny=1;
                    int l,f;
                    losuj_kordy(&l,&f);
                    zmienna->x=l;
                    zmienna->y=f;
                    losuj_kordy(&l,&f);
                    zmienna3->x=l;
                    zmienna3->y=l;
                    zmienna->last_sign=' ';
                    zmienna3->last_sign=' ';
                    zmienna->deaths+=1;
                    zmienna3->deaths+=1;
                    zn=' ';
                }
            }
            else if(zn=='4')
            {
                if(zmienna->gold_carry==0 && zmienna4->gold_carry==0)
                {
                    int m_x=zmienna->x,m_y=zmienna->y;
                    if(basic_tab[m_x][m_y]=='#') tab[m_x][m_y]='#';
                    else tab[m_x][m_y]=' ';
                    tab[camp_x][camp_y]='A';
                    int l,f;
                    losuj_kordy(&l,&f);
                    zmienna->x=l;
                    zmienna->y=f;
                    losuj_kordy(&l,&f);
                    zmienna4->x=l;
                    zmienna4->y=l;
                    zmienna->last_sign=' ';
                    zmienna4->last_sign=' ';
                    zmienna->deaths+=1;
                    zmienna4->deaths+=1;
                    zn=' ';
                }
                else
                {
                    int q=0;
                    for(q=0;q<20;q++)
                    {
                        if(ciala[q].aktywny==0) break;
                    }
                    ciala[q].hajs=zmienna->gold_carry+zmienna4->gold_carry;
                    zmienna->gold_carry=0;
                    zmienna4->gold_carry=0;
                    tab[zmienna->x][zmienna->y]='D';
                    ciala[q].x=zmienna->x;
                    ciala[q].y=zmienna->y;
                    ciala[q].aktywny=1;
                    int l,f;
                    losuj_kordy(&l,&f);
                    zmienna->x=l;
                    zmienna->y=f;
                    losuj_kordy(&l,&f);
                    zmienna4->x=l;
                    zmienna4->y=l;
                    zmienna->last_sign=' ';
                    zmienna4->last_sign=' ';
                    zmienna->deaths+=1;
                    zmienna4->deaths+=1;
                    zn=' ';
                }
            }
            else if(zn=='*')
            {
                zn=' ';             
            }
            if(i==0) zmienna1->last_sign=zn;
            else if(i==1) zmienna2->last_sign=zn;
            else if(i==2) zmienna3->last_sign=zn;
            else if(i==3) zmienna4->last_sign=zn;
            int p=-2;
            for(int i=0;i<5;i++,p++)
            {
                int g=-2;
                for(int j=0;j<5;j++,g++)
                {
                    zmienna->tablica[i][j]=tab[zmienna->x+p][zmienna->y+g];
                }
            }
            zmienna->rounds+=1;
        }
        runda+=1;
        usleep(1000000);
        
        sem_post(sem1);
        
    }
    return NULL;
}
void *konczenie(void *par)
{
    while(1)
    {
        sem_wait(semafor_program);
        int place=*info;
        place-=1;
        if(place==0)
        {
            if(basic_tab[zmienna1->x][zmienna1->y]=='#') tab[zmienna1->x][zmienna1->y]='#';
            else tab[zmienna1->x][zmienna1->y]=' ';
        }
        else if(place==1)
        {
            if(basic_tab[zmienna2->x][zmienna2->y]=='#') tab[zmienna2->x][zmienna2->y]='#';
            else tab[zmienna2->x][zmienna2->y]=' ';            
        }
        else if(place==2)
        {
            if(basic_tab[zmienna3->x][zmienna3->y]=='#') tab[zmienna3->x][zmienna3->y]='#';
            else tab[zmienna3->x][zmienna3->y]=' ';            
        }
        else if(place==3)
        {
            if(basic_tab[zmienna4->x][zmienna4->y]=='#') tab[zmienna4->x][zmienna4->y]='#';
            else tab[zmienna4->x][zmienna4->y]=' ';            
        }
        tab[camp_x][camp_y]='A';
        gracze[place]=0;
        ilosc_graczy-=1;
        sem_post(semafor_koniec_programu);
    }
    return NULL;
}

void *sprawdz_zycie(void *par)
{
    while(1)
    {
        if(gracze[0]==1)
        {
            sprawdz1->dzwignia=-1;
            sprawdz1->wystartowal=1;
        }
        if(gracze[1]==1)
        {
            sprawdz2->dzwignia=-1;
            sprawdz2->wystartowal=1;
        }
        if(gracze[2]==1)
        {
            sprawdz3->dzwignia=-1;
            sprawdz3->wystartowal=1;            
        }
        if(gracze[3]==1)
        {
            sprawdz4->dzwignia=-1;
            sprawdz4->wystartowal=1;            
        }
        usleep(1000000);
        if(sprawdz1->wystartowal==1 && sprawdz1->dzwignia==-1)
        {
            if(basic_tab[zmienna1->x][zmienna1->y]=='#') tab[zmienna1->x][zmienna1->y]='#';
            else tab[zmienna1->x][zmienna1->y]=' ';
            tab[camp_x][camp_y]='A';
            gracze[0]=0;
            ilosc_graczy-=1;
        }
        if(sprawdz2->wystartowal==1 && sprawdz2->dzwignia==-1)
        {
            if(basic_tab[zmienna2->x][zmienna2->y]=='#') tab[zmienna2->x][zmienna2->y]='#';
            else tab[zmienna2->x][zmienna2->y]=' ';   
            tab[camp_x][camp_y]='A';
            gracze[1]=0;
            ilosc_graczy-=1;             
        }
        if(sprawdz3->wystartowal==1 && sprawdz3->dzwignia==-1)
        {
            if(basic_tab[zmienna3->x][zmienna3->y]=='#') tab[zmienna3->x][zmienna3->y]='#';
            else tab[zmienna3->x][zmienna3->y]=' ';     
            tab[camp_x][camp_y]='A';
            gracze[2]=0;
            ilosc_graczy-=1;          
        }
        if(sprawdz4->wystartowal==1 && sprawdz4->dzwignia==-1)
        {
            if(basic_tab[zmienna4->x][zmienna4->y]=='#') tab[zmienna4->x][zmienna4->y]='#';
            else tab[zmienna4->x][zmienna4->y]=' ';      
            tab[camp_x][camp_y]='A';
            gracze[3]=0;
            ilosc_graczy-=1;         
        }
        sprawdz1->wystartowal=0;
        sprawdz2->wystartowal=0;
        sprawdz3->wystartowal=0;
        sprawdz4->wystartowal=0;
    }
    return NULL;
}

int main ()
{
	initscr();
	noecho();
    has_colors();
    start_color();
	curs_set(FALSE);
    sem_init(&semafor_bestia,0,1);
    srand(time(NULL));
    
    semafor_koniec_pobran=sem_open("pobieranie_end",O_CREAT|O_EXCL,0600,0);
    semafor_pobran=sem_open("pobieranie",O_CREAT|O_EXCL,0600,0);
    
    int ppp=shm_open("memory",O_CREAT|O_RDWR,0666);
    ftruncate(ppp,sizeof(int));
    informacja=(int*)mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,ppp,0);

    pthread_t klienty;
    pthread_create(&klienty,NULL,przyjmowanie_klientow,NULL);
    
    
    
    
    semafor_program=sem_open("program_end",O_CREAT|O_EXCL,0600,0);
    semafor_koniec_programu=sem_open("program",O_CREAT|O_EXCL,0600,0);
    
    int size=shm_open("konczenie",O_CREAT|O_RDWR,0666);
    ftruncate(size,sizeof(int));
    info=(int*)mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,size,0);
    
    pthread_t klienty_konczenie;
    pthread_create(&klienty_konczenie,NULL,konczenie,NULL);
    
    
    
    
    sem1=sem_open("semafor",O_CREAT|O_EXCL,0600,3);
    sem2=sem_open("semafor2",O_CREAT|O_EXCL,0600,3);//serwerowy
    
    

    int fd1=shm_open("memory1",O_CREAT|O_RDWR,0666);
    ftruncate(fd1,sizeof(struct send));
    zmienna1=(struct send*)mmap(NULL,sizeof(struct send),PROT_READ|PROT_WRITE,MAP_SHARED,fd1,0);
    
    int fd2=shm_open("memory2",O_CREAT|O_RDWR,0666);
    ftruncate(fd2,sizeof(struct send));
    zmienna2=(struct send*)mmap(NULL,sizeof(struct send),PROT_READ|PROT_WRITE,MAP_SHARED,fd2,0);
        
    int fd3=shm_open("memory3",O_CREAT|O_RDWR,0666);
    ftruncate(fd3,sizeof(struct send));
    zmienna3=(struct send*)mmap(NULL,sizeof(struct send),PROT_READ|PROT_WRITE,MAP_SHARED,fd3,0);
    
    int fd4=shm_open("memory4",O_CREAT|O_RDWR,0666);
    ftruncate(fd4,sizeof(struct send));
    zmienna4=(struct send*)mmap(NULL,sizeof(struct send),PROT_READ|PROT_WRITE,MAP_SHARED,fd4,0);
    


    int fd5=shm_open("check1",O_CREAT|O_RDWR,0666);
    ftruncate(fd5,sizeof(struct zywot));
    sprawdz1=(struct zywot*)mmap(NULL,sizeof(struct zywot),PROT_READ|PROT_WRITE,MAP_SHARED,fd5,0);
    
    int fd6=shm_open("check2",O_CREAT|O_RDWR,0666);
    ftruncate(fd6,sizeof(struct zywot));
    sprawdz2=(struct zywot*)mmap(NULL,sizeof(struct zywot),PROT_READ|PROT_WRITE,MAP_SHARED,fd6,0);
        
    int fd7=shm_open("check3",O_CREAT|O_RDWR,0666);
    ftruncate(fd7,sizeof(struct zywot));
    sprawdz3=(struct zywot*)mmap(NULL,sizeof(struct zywot),PROT_READ|PROT_WRITE,MAP_SHARED,fd7,0);
    
    int fd8=shm_open("check4",O_CREAT|O_RDWR,0666);
    ftruncate(fd8,sizeof(struct zywot));
    sprawdz4=(struct zywot*)mmap(NULL,sizeof(struct zywot),PROT_READ|PROT_WRITE,MAP_SHARED,fd8,0);
    
    
    pthread_mutex_init(&mut,NULL);
    dodajemy_zmienna();
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    init_pair(2, COLOR_YELLOW, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    init_pair(6, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(7, COLOR_RED, COLOR_WHITE);
    init_pair(8, COLOR_BLACK, COLOR_BLACK);
    narysuj();

    pthread_t display;
    pthread_create(&display,NULL,drukowanie,NULL);
    pthread_t control;
    pthread_create(&control,NULL,kierowanie,NULL);
    pthread_t czy_zyje;
    pthread_create(&czy_zyje,NULL,sprawdz_zycie,NULL);
    pthread_t keyboard;
    pthread_create(&keyboard,NULL,klawiatura,NULL);
    pthread_join(keyboard,NULL);

	
	refresh();
    sleep(1);
    sem_close(sem1);
    sem_close(sem2);
    sem_close(semafor_pobran);
    sem_close(semafor_koniec_pobran);
    sem_close(semafor_program);
    sem_close(semafor_koniec_programu);
    
    munmap(informacja,sizeof(int));
    close(ppp);
    shm_unlink("memory");
    
    
    
    munmap(zmienna1,sizeof(struct send));
    close(fd1);
    shm_unlink("memory1");
    
    munmap(zmienna1,sizeof(struct send));
    close(fd2);
    shm_unlink("memory2");
    
    munmap(zmienna3,sizeof(struct send));
    close(fd3);
    shm_unlink("memory3");
    
    munmap(zmienna4,sizeof(struct send));
    close(fd4);
    shm_unlink("memory4");
    
    
    
    munmap(sprawdz1,sizeof(struct zywot));
    close(fd5);
    shm_unlink("check1");
    
    munmap(sprawdz2,sizeof(struct zywot));
    close(fd6);
    shm_unlink("check2");
    
    munmap(sprawdz3,sizeof(struct zywot));
    close(fd7);
    shm_unlink("check3");
    
    munmap(sprawdz4,sizeof(struct zywot));
    close(fd8);
    shm_unlink("check4");
    
    
    munmap(info,sizeof(int));
    close(size);
    shm_unlink("konczenie");
    
    
    sem_unlink("pobieranie");
    sem_unlink("pobieranie_end");
    sem_unlink("semafor");
    sem_unlink("semafor2");
    sem_unlink("program_end");
    sem_unlink("program");


    sem_close(&semafor_bestia);
    pthread_mutex_destroy(&mut);
    endwin();
    
	return 0;	
}
