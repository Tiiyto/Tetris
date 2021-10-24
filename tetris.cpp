#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <sstream>

using namespace std;
using namespace sf;

const int hauteurAire =20;
const int largAire =15;
int aireJeu [hauteurAire][largAire];

const int coteCarre=25;
const int tailleText=22;
const int largT=3;

struct coordonnes
{
    int x;
    int y;

}a[4],b[4]; //

int forme[7][4] =
{
	1,3,5,7, // I
    2,3,4,5, // O
	2,4,5,7, // Z
    2,3,5,7, // L
	3,5,4,6, // S
	3,5,4,7, // T
	3,5,7,6, // J

};

bool verification ()
{
    for (int i=0;i<4;i++)
    {
        if( a[i].x  <0 || a[i].x >=largAire || a[i].y>=hauteurAire)
        {
            return false;
        }
        else if (aireJeu[a[i].y][a[i].x])
            {return false;}
    }
    return true;
}

void pDecalage (int decalagex)
{
    /*Décale la pièce */
    for (int i=0;i<4;i++)
    {
        b[i]=a[i]; // sauvegarde les composants
        a[i].x=a[i].x+decalagex;
    }
    /* Si la véirication n'est pas bonne remets les composants d'origine */
    if (!verification())
    for (int i=0;i<4;i++)
    {a[i]=b[i];}
}

void pRotation ()
{
        coordonnes centreRotation = a[1];
        /* Change les composants des carrés afin d'effectuer une rotation */

		for (int i=0;i<4;i++)
		  {
			int x = a[i].y - centreRotation.y;
			int y = a[i].x - centreRotation.x;

			a[i].x = centreRotation.x - x;
			a[i].y = centreRotation.y + y;
	 	  }
        /* Si la véirication n'est pas bonne remets les composants d'origine */
   	    if (!verification())
        {
            for (int i=0;i<4;i++)
            {a[i]=b[i];}
        }
}

void pDescente ( float & timer,float & score)
{
    /* Descent la pièce et augmente le score */
    for (int i=0;i<4;i++)
    {
        b[i]=a[i];
        a[i].y=a[i].y+1;
        score=score+0.1;
    }
        /* vérifie si l'on peux descendre */
		if (!verification())
		{
		 for (int i=0;i<4;i++)
		 {
		     aireJeu[b[i].y][b[i].x]=1; //remplit l'aire de jeu afin de savoir qu'une pièce si trouve
		 }

		 int n=rand()%7;
		 /* Definit les composants de la prochaine forme*/
		 for (int i=0;i<4;i++)
		   {
            a[i].y = forme[n][i] / 2;
		    a[i].x = forme[n][i] % 2;
		   }
		}

	  	timer=0;// réinitialise le timer

}

void pVerificationLignes (int & decalagex, bool & rotation, float & delais)
{
    int k= hauteurAire-1;
    /* Vérifie chaque ligne */
	for (int i=hauteurAire-1;i>0;i--)
	{
		int c=0; // initialise un compteur
		/* Verifie chaque colonne */
		for (int j=0;j<largAire;j++)
		{
		    if (aireJeu[i][j]) {c++;}
		    aireJeu[k][j]=aireJeu[i][j];
		}
		if (c<largAire)
        {
            k--;
        }
	}

    /* réinitialise les variables */
    decalagex=0;
    rotation=false;
    delais=0.3;

}

void pDessin (RectangleShape & rect, RenderWindow & window)
{
    for (int i=0;i<hauteurAire;i++)
	 for (int j=0;j<largAire;j++)
	   {
         if (aireJeu[i][j]==0)
         {continue;}// continue la boucle sans traiter le cas où l'aire de jeu est vide
		 rect.setPosition(j*coteCarre,i*coteCarre);
		 rect.move(coteCarre*2,coteCarre);  // pour ne pas commencer a 0.0
		 window.draw(rect);
	   }

	for (int i=0;i<4;i++)
	  {
		rect.setPosition(a[i].x*coteCarre,a[i].y*coteCarre);
		rect.move(coteCarre*2,coteCarre);// pour ne pas commencer a 0.0
		window.draw(rect);
	  }

}

void pfondJeu (RenderWindow & window, double score)
{
    /* Convertir un int en string */
    ostringstream s;
    s << score;
    string leScore= "Score : " + s.str();

    Font font;
    font.loadFromFile("arial.ttf");
     /*Menu de jeu */
    Text text[3];

    text[0].setFont(font);
    text[0].setString("Niveau : 1");
    text[0].setCharacterSize(tailleText);
    text[0].setFillColor(Color::White);
    text[0].setPosition(550,150);

    text[1].setFont(font);
    text[1].setString(leScore);
    text[1].setCharacterSize(tailleText);
    text[1].setFillColor(Color::White);
    text[1].setPosition(550,300);

    text[2].setFont(font);
    text[2].setString("Lignes : ");
    text[2].setCharacterSize(tailleText);
    text[2].setFillColor(Color::White);
    text[2].setPosition(550,450);

    Text Pause;
    Pause.setFont(font);
    Pause.setString("Pour mettre pause au jeu, veuillez appuyer sur la touche echap");
    Pause.setCharacterSize(tailleText);
    Pause.setFillColor(Color::White);
    Pause.setPosition(80,550);

    Text jouer;
    jouer.setFont(font);
    jouer.setString("Jouer");
    jouer.setCharacterSize(tailleText);
    jouer.setFillColor(Color::Black);
    jouer.setPosition(610,50);

    /* bouton jouer*/
    RectangleShape boutonJouer;
    boutonJouer.setSize(Vector2f(100,30));
    boutonJouer.setFillColor(Color::White);
    boutonJouer.setPosition(590,50);

    /* espace de jeu */
    RectangleShape cadreJeu[4];
    cadreJeu[0].setSize(Vector2f(largAire*coteCarre,largT));
    cadreJeu[0].setFillColor(Color::White);
    cadreJeu[0].setPosition(50,coteCarre-largT);

    cadreJeu[1].setSize(Vector2f(coteCarre*hauteurAire+largT*2,largT));
    cadreJeu[1].setFillColor(Color::White);
    cadreJeu[1].setPosition(50,coteCarre-largT);
    cadreJeu[1].rotate(90);

    cadreJeu[2].setSize(Vector2f(coteCarre*hauteurAire+largT*2,largT));
    cadreJeu[2].setFillColor(Color::White);
    cadreJeu[2].setPosition(cadreJeu[0].getPosition().x+largAire*coteCarre+largT,coteCarre-largT);
    cadreJeu[2].rotate(90);

    cadreJeu[3].setSize(Vector2f(largAire*coteCarre,largT));
    cadreJeu[3].setFillColor(Color::White);
    cadreJeu[3].setPosition(50,(coteCarre*(hauteurAire+1)));

         for (int j=0;j<4;j++)
       {
           window.draw(text[j]);
           window.draw(boutonJouer);
           window.draw(jouer);
           window.draw(cadreJeu[j]);
           window.draw(Pause);
       }
}

void boutonJouer (RenderWindow & window,int & jeu )
{
    if (Mouse::getPosition(window).x>=590 && Mouse::getPosition(window).x<=690 && Mouse::getPosition(window).y>=50 && Mouse::getPosition(window).y<=80)
        {
            if (jeu==0){jeu++;}
        }
}

 int main (void)
 {
    RenderWindow window (VideoMode(800,600,64), "Tetris");
    Clock clock;
    RectangleShape rect ;
    srand(time(0));

    rect.setSize(Vector2f(coteCarre,coteCarre));
    rect.setFillColor(Color::Red);

    float score=0;
    int jeu=0;
    int decalagex=0;
    bool rotation = false;
    float timer=0;
    float delais=0.3;
    /* Definition des premières coordonnées ici, le I */
    a[0] = {1,1};
    a[1] = {2,1};
    a[2] = {3,1};
    a[3] = {4,1};

        while (window.isOpen())
    {
        Event event;
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer=timer+time;
        while (window.pollEvent(event))
        {

            switch(event.type)
            {
            case Event::Closed:
                window.close();
            break;
            case Event::MouseButtonReleased:
                boutonJouer(window,jeu);
            break;
            case Event::KeyPressed:
                switch (event.key.code)
                {
                case Keyboard::Right:
                    decalagex=1;
                    break;

                case Keyboard::Left:
                    decalagex=-1;
                    break;

                case Keyboard::Up:
                    rotation=true;
                    break;
                 case Keyboard::Return:
                    jeu=1;
                    break;
                 case Keyboard::Down:
                    delais=0.1;
                    break;
                 case Keyboard::Escape:
                    if (jeu==1)
                    {jeu=0;}
                    else {jeu=1;}
                break;
                }
                break;
            }
            break;
        }

    if (jeu==1)
    {
        if (timer>delais){pDescente(timer,score);}
        pDecalage(decalagex);
        if (rotation){pRotation();}
    }

    pVerificationLignes(decalagex,rotation,delais);

    window.clear();
    pDessin(rect,window);
    pfondJeu(window,score);
    window.display();

    }
    return 0;
 }
