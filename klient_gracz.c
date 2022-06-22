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


//STEROWANIE ZA POMOCA KLAWISZY WASD/wasd

sem_t *semafor_pobran;
sem_t *semafor_koniec_pobran;

sem_t *semafor_koniec_programu;
sem_t *semafor_program;

struct zywot
{
    int wystartowal;
    int dzwignia;
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

struct zywot *przelacznik=NULL;

void *sprawdz_life(void *par)
{
    while(1)
    {
        przelacznik->dzwignia=1;
        usleep(100000);
    }
    return NULL;
}

void *drukowanie(void* par)
{
    int c_x=-1,c_y=-1;
    while(1)
    {
        if((((struct send*)par)->zabity)==1)
        {
            (((struct send*)par)->zabity)=0;
            for(int i=0;i<27;i++)
            {
                for(int j=0;j<53;j++)
                {
                    attron(COLOR_PAIR(3));
                    mvprintw(i,j," ");
                    refresh();
                    attroff(COLOR_PAIR(3));    
                }
            }
        }
        int p=(((struct send*)par)->x)-2;
        for(int i=0;i<5;i++,p++)
        {
            int g=(((struct send*)par)->y)-2;
            for(int j=0;j<5;j++,g++)
            {
                if((((struct send*)par)->tablica[i][j])=='c' || (((struct send*)par)->tablica[i][j])=='t' || (((struct send*)par)->tablica[i][j])=='T' )
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(p,g,"%c",((struct send*)par)->tablica[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(1));
                }
                else if((((struct send*)par)->tablica[i][j])=='A')
                {
                    attron(COLOR_PAIR(2));
                    mvprintw(p,g,"%c",((struct send*)par)->tablica[i][j]);
                    c_x=p;
                    c_y=g;
                    refresh();
                    attroff(COLOR_PAIR(2));                   
                }
                else if((((struct send*)par)->tablica[i][j])=='%')
                {
                    attron(COLOR_PAIR(3));
                    mvprintw(p,g,"%c",((struct send*)par)->tablica[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(3));                   
                }
                else if((((struct send*)par)->tablica[i][j])==' ')
                {
                    attron(COLOR_PAIR(4));
                    mvprintw(p,g,"%c",((struct send*)par)->tablica[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(4));                   
                }
                else if((((struct send*)par)->tablica[i][j])=='#')
                {
                    attron(COLOR_PAIR(5));
                    mvprintw(p,g,"%c",((struct send*)par)->tablica[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(5));                   
                }
                else if((((struct send*)par)->tablica[i][j])=='1' || (((struct send*)par)->tablica[i][j])=='2' || (((struct send*)par)->tablica[i][j])=='3' || (((struct send*)par)->tablica[i][j])=='4')
                {
                    attron(COLOR_PAIR(6));
                    mvprintw(p,g,"%c",((struct send*)par)->tablica[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(6));                   
                }
                else if((((struct send*)par)->tablica[i][j])=='*')
                {
                    attron(COLOR_PAIR(7));
                    mvprintw(p,g,"%c",((struct send*)par)->tablica[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(7));                   
                }
                else if((((struct send*)par)->tablica[i][j])=='D')
                {
                    attron(COLOR_PAIR(2));
                    mvprintw(p,g,"%c",((struct send*)par)->tablica[i][j]);
                    refresh();
                    attroff(COLOR_PAIR(2));     
                }
                else
                {
                    attron(COLOR_PAIR(3));
                    mvprintw(p,g," ");
                    refresh();
                    attroff(COLOR_PAIR(3));
                }
            }
        }
            mvprintw(3,55,"Server's PID: %d",((struct send*)par)->servers_id);
            if(c_x!=-1) 
            {
                mvprintw(4,56,"Campsite X/Y:              ");
                refresh();
                mvprintw(4,56,"Campsite X/Y: %02d/%02d",c_x,c_y);
            }
            else mvprintw(4,56,"Campsite X/Y: UNKNOWN!");
            mvprintw(5,56,"Round numer: %d",((struct send*)par)->rounds);
            mvprintw(7,55,"Player:  ");
            mvprintw(8,55," Number: %c",((struct send*)par)->player_number);
            if(((struct send*)par)->type==0) mvprintw(9,55," Type: BOT");
            else mvprintw(9,55," Type:   HUMAN");
            
            mvprintw(10,55," Curr: %02d/%02d",((struct send*)par)->x,((struct send*)par)->y);
            mvprintw(11,55," Deaths: %d",((struct send*)par)->deaths);
            mvprintw(13,55," Coins");
            mvprintw(14,55,"     carried: %04d",((struct send*)par)->gold_carry);
            mvprintw(15,55,"     brought: %04d",((struct send*)par)->bank);
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
            usleep(100000);
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
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    init_pair(2, COLOR_YELLOW, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    init_pair(6, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(7, COLOR_RED, COLOR_WHITE);
    
    semafor_koniec_pobran=sem_open("pobieranie_end",O_CREAT);
    semafor_pobran=sem_open("pobieranie",O_CREAT);
    
    int ppp=shm_open("memory",O_CREAT|O_RDWR,0666);
    ftruncate(ppp,sizeof(int));
    int *informacja=(int*)mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,ppp,0);
    
    sem_post(semafor_pobran);
    sem_wait(semafor_koniec_pobran);
    
    int liczba=*informacja;
    sem_close(semafor_pobran);
    sem_close(semafor_koniec_pobran);
    munmap(informacja,sizeof(int));
    close(ppp);
    
    if(liczba==-1)
    {
        mvprintw(0,0,"NIE MA MIEJSCA DLA CIEBIE NA SERWERZE! SPROBUJ POZNIEJ!");
        refresh();
        sleep(5);
        endwin();
        return 0;
    }
    
    char nazwa[]="memoryX";
    nazwa[6]=liczba+48;
	sem_t* sem1=sem_open("semafor",O_CREAT);
	sem_t* sem2=sem_open("semafor2",O_CREAT);
    int fd = shm_open(nazwa,O_CREAT|O_RDWR,0666);
    ftruncate(fd,sizeof(struct send));
    
    char name[]="checkX";
    name[5]=liczba+48;
    int fd2= shm_open(name,O_CREAT|O_RDWR,0666);;
    ftruncate(fd2,sizeof(struct zywot));
    przelacznik=(struct zywot*)mmap(NULL,sizeof(struct zywot),PROT_READ|PROT_WRITE,MAP_SHARED,fd2,0);
    
    
    struct send* zmienna=(struct send*)mmap(NULL,sizeof(struct send),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    sleep(1);
    zmienna->id=getpid();
    zmienna->type=1;
    refresh();
    
    pthread_t disp;
    pthread_create(&disp,NULL,drukowanie,(void*)zmienna);
    pthread_t life;
    pthread_create(&life,NULL,sprawdz_life,NULL);
    sleep(1);
    char znaczek;
	while(1)
    {
        znaczek=' ';
        znaczek=getch();
        if(znaczek=='s' || znaczek=='S') zmienna->ruch='B';
        else if(znaczek=='w' || znaczek=='W') zmienna->ruch='A';
        else if(znaczek=='D' || znaczek=='d') zmienna->ruch='C';
        else if(znaczek=='A' || znaczek=='a') zmienna->ruch='D';
        else if(znaczek==' ') zmienna->ruch=' ';
        else
        {
            zmienna->ruch='q';
            sem_post(sem2);
            break;          
        }
        sem_post(sem2);
    }
    zmienna->graczy-=1;
	sem_close(sem1);
	sem_close(sem2);
    munmap(zmienna,sizeof(struct send));
    close(fd);
    munmap(przelacznik,sizeof(struct zywot));
    close(fd2);

    semafor_program=sem_open("program_end",O_CREAT);
    semafor_koniec_programu=sem_open("program",O_CREAT);
    
    int size=shm_open("konczenie",O_CREAT|O_RDWR,0666);
    ftruncate(size,sizeof(int));
    int *info=(int*)mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,size,0);
    *info=liczba;
    sem_post(semafor_program);
    sem_wait(semafor_koniec_programu);
    
    sem_close(semafor_program);
    sem_close(semafor_koniec_programu);
    munmap(info,sizeof(int));
    close(size);
    endwin();
	return 0;
}