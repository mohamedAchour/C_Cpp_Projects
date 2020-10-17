///////////////////////////////////////////////////////////////
////////CORDIC_cosinus_sinus_fixed-point_arithmetic////////////
/////////////////ACHOUR_MOHAMED_29/03/2020/////////////////////
///////////////////////////////////////////////////////////////

#include<iostream>
#include<cmath>
#include<fstream>

//nombre d iterations maximal
#define MAX_ITER 64

//définitions des facteurs multiplicateurs:
#define FACT_MUL_1 32767
#define FACT_MUL_2 353.35

using namespace std;

//la fonction de codage en virgule fixe
short int fp2fix(double M, double x)
{
  short int val=0;
    val=(short int)(x*M);
    return val;
}
//la fonction de décodage
double fix2fp(double M, short int x)
{
    return x/M;
}

//tableau pour stocker les angles theta_i (operation a faire une fois pour toute)
double theta[MAX_ITER];
//tableau pour stocker les termes An=theta_0*theta_1....*theta_n (operation a faire une fois pour toute)
double A[MAX_ITER];

//cette parte nest pas synthesise sur FPGA
void intTab(void)
{
    ofstream thetaTable("E:/POLY-UCA/cours/S8/ESN/TP_CORDIC/C_virgule_fixe/theta_i.txt");
    ofstream A_table("E:/POLY-UCA/cours/S8/ESN/TP_CORDIC/C_virgule_fixe/A_table.txt");

    double d=1;
    //on stocke la premiere valeur (le terme *M_PI/180 pour faire la conversion de l angle au radian car cmath utilise le radian )
    A[0]=fp2fix(FACT_MUL_1,cos(45*M_PI/180));

    A_table<<"(";
    thetaTable<<"(";
    for(int i(0);i<MAX_ITER;i++)
    {
        //les angles theta_i sont des tangentes inverse des 1/d avec d puissance de deux(tan(theta_i=2^(i) i=0,1,2....)
        theta[i]=fp2fix(FACT_MUL_2,180/M_PI*atan(d));
        if(i>0)
        {
            //A[i]=A[0]*A[1]....*A[i]
            A[i]=fp2fix(FACT_MUL_1,fix2fp(FACT_MUL_1,A[i-1])*cos(fix2fp(FACT_MUL_2,theta[i])*M_PI/180));
        }

        d=d/2;

        //remplir les fichiers .txt
        if(i<MAX_ITER-1)
        {

            A_table<<A[i]<<",";
            thetaTable<<theta[i]<<",";

            //pour avoir cinq valeurs par ligne
            if((i+1)%(5)==0)
            {
                A_table<<endl;
                thetaTable<<endl;
            }
        }
    }
    //on remplie la dernire valeur et on ferme la table ");" pour avoir a la fin "(..les valeurs..);"
    A_table<<A[MAX_ITER-1]<<");";
    thetaTable<<theta[MAX_ITER-1]<<");";

}

//la fonction synthetise sur FPGA
void cordic(short int niter,short int phi,short int *rcos,short int *rsin)
{
    short int x,y,new_x,new_y,alpha;

    //conditionner la valeur du nombre des itérations
    niter = (niter>MAX_ITER) ? MAX_ITER : niter;

    //initialisation des parametres
    x=A[niter];
    y=0;
    alpha=0;

    //l implementation suivante vient des resultats mathematiques de l algorithme du cordic
    for(int i(0);i<niter;i++)
    {
        if(alpha<phi)
        {
            alpha=alpha+theta[i];
            new_x=(x-(y>>i));
            new_y=(y+(x>>i));
            //memorisation de x et y
            x = new_x; y = new_y;
        }
        else
        {
            alpha=alpha-theta[i];
            new_x=(x+(y>>i));
            new_y=(y-(x>>i));
            //memorisation de x et y
            x = new_x; y = new_y;
        }

    }

    //le cos et le sin prennent la derniere valeur calcule de x et y
    *rcos=x;
    *rsin=y;
}

int main()
{
    //les variables pour stcker les valeurs du sinus et du cosinus
    short int rsin,rcos;

    //l angle phi donnée par l'utilisateur
    short int phi=0;
    short int niter;

    //appel de la fonction d initialisation des tables
    intTab();

    cout<<"Saisir une angle: "<<endl;
    cin>>phi;
    cout<<"Saisir le nombre des iterations: "<<endl;
    cin>>niter;

    cordic(niter,fp2fix(FACT_MUL_2,phi),&rcos,&rsin);
    cout<<"\n\ncos("<<phi<<")= "<<fix2fp(FACT_MUL_1,16376)<<" sin("<<phi<<")= "<<fix2fp(FACT_MUL_1,28368)<<endl;

    return 0;
}

