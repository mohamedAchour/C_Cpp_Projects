///////////////////////////////////////////////////////////////////////////////////
//////////////////////this code is expected to run on :////////////////////////////
//////////////////dsPIC33FJ32GP202/204 and dsPIC33FJ16GP304////////////////////////
///////////////////////////////////////////////////////////////////////////////////



#define    rad_to_deg  180/3.141592654; //  coefficient de conversion de l'angle en radian à l'angle en degré
#define    delta_t  0.02  // delta_t c'est ~0.02 c'est le temp d'interruption du timer1



//////////////DECLARATION DES VARIABLES//////////////

int   Acc_rawX, Acc_rawY, Acc_rawZ, Gyr_rawX;
float Angle_acc=0;
float Angle_gyro=0;
float angle_totale;
char  mot_recu =0;
int   mot_compteur=0;
float PID, pwm_g, pwm_d, erreur, erreur_prec,vitesse_moteur_g,vitesse_moteur_d;
int   vitesse_rot=30,vitesse_max=150;
float pid_p=0;
float pid_i=0;
float pid_d=0;
////////////////////////constante du PID /////////////////////
float kp=25;
float ki=0;
float kd=0.8;
float angle_desire = 0;
float vitesse=0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////**************************************//////////////////////////////////////
//////////////////////////////////////Extraction des données d'accéléromètre//////////////////////////////////////
//////////////////////////////////////**************************************//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int   acc_x(){
              int valeur=0;  // la valeur ou on va stocker les données
              I2C1_Start();
              I2C1_Write(0x68);
              I2C1_Write(0x3B);
              I2C1_Restart();
              I2C1_Write(0x69);// on met le bit R/W à 1 pour dire qu'on veut faire une lecture et ainsi on obtient 69 (adresse du MPU +1)
              valeur=I2C1_Read(1)<<8|I2C1_Read(0); // on stock 16 bit  (8 bit du registre 3B ACCEL_XOUT[15:8] et du registre 3C ACCEL_XOUT[7:0]
                                                   //l'incrémentation au registre 3C est faite automatiquement
               return valeur;
              }

int   acc_y(){
              int valeur=0;
              I2C1_Start();
              I2C1_Write(0x68);
              I2C1_Write(0x3D);
              I2C1_Restart();
              I2C1_Write(0x69);// on met le bit R/W à 1 pour dire qu'on veut faire une lecture et ainsi on obtient 69 (adresse du MPU +1)
              valeur=I2C1_Read(1)<<8|I2C1_Read(0); //  on stock 16 bit  (8 bit du registre 3D ACCEL_YOUT[15:8] et du registre 3E ACCEL_YOUT[7:0])
                                                   //l'incrémentation au registre 3E est faite automatiquement
               return valeur;
              }

int   acc_z(){
              int valeur=0;
              I2C1_Start();
              I2C1_Write(0x68);
              I2C1_Write(0x3F);
              I2C1_Restart();
              I2C1_Write(0x69);// on met le bit R/W à 1 pour dire qu'on veut faire une lecture et ainsi on obtient 69 (adresse du MPU +1)
              valeur=I2C1_Read(1)<<8|I2C1_Read(0); //  on stock 16 bit  (8 bit du registre 3F ACCEL_ZOUT[15:8] et du registre 40 ACCEL_ZOUT[7:0])
                                                    //l'incrémentation au registre 40 est faite automatiquement
               return valeur;
              }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////**************************************//////////////////////////////////////
//////////////////////////////////////Extraction des données du gyroscope/////////////////////////////////////////
//////////////////////////////////////**************************************//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int   gyro_x(){
              int valeur=0;
              I2C1_Start();
              I2C1_Write(0x68);
              I2C1_Write(0x43);
              I2C1_Restart();
              I2C1_Write(0x69);// on met le bit R/W à 1 pour dire qu'on veut faire une lecture et ainsi on obtient 69 (adresse du MPU +1)
              valeur=I2C1_Read(1)<<8|I2C1_Read(0); //  on stock 16 bit  (8 bit du registre 43 GYRO_XOUT[15:8] et du registre 44 GYRO_XOUT[7:0])
               return valeur;                      //l'incrémentation au registre 44 est faite automatiquement
              }



//////////////////////////////////////*****************************************************//////////////////////////////////////
//////////////////////////////////////**********définir le sens de rotation des moteurs************/////////////////////////////
//////////////////////////////////////*****************************************************//////////////////////////////////////
void avancer(){
              RB10_bit=0;    // les deux moteurs tourne toujours dans le meme sens alors on connecte les pins IN1 et IN3 à 0 (les pins du L293D)
              RB11_bit=1;}   //  on connecte les pins IN2 et IN4 à 1

void reculer(){
              RB10_bit=1;  //les deux moteurs tourne toujours dans le meme sens alors on connecte les pins IN1 et IN3 à 1 (les pins du L293D)
              RB11_bit=0;} // on connecte les pins IN2 et IN4 à 0

//////////////////////////////////////*******************************************//////////////////////////////////////
//////////////////////////////////////**********le calcul du duty_cycle**********//////////////////////////////////////
//////////////////////////////////////*******************************************//////////////////////////////////////


//////////////////////////////////////*****************************************************//////////////////////////////////////
//////////////////////////////////////****************le routine du timer1*****************//////////////////////////////////////
//////////////////////////////////////*****************************************************//////////////////////////////////////
void TM1() iv IVT_ADDR_T1INTERRUPT ics ICS_AUTO {
                                                pwm_d=(49999*(vitesse_moteur_d))/(150);  //on a pour vitsse max ----> 49999 d'ou pour notre vitesse la ralation
                                                pwm_g=(49999*(vitesse_moteur_g))/(150);
                                                OC1RS =pwm_g; // à chaque ~20ms la vitesse du moteur gauche est ajustée
                                                OC2RS =pwm_d; //  à chaque ~20ms la vitesse du moteur droite est ajustée
                                                if(angle_totale<0){reculer();}
                                                else if(angle_totale>0){avancer();}

                                                 T1IF_bit=0;
                                                }


///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////**************************************///////////////////////////
//////////////////////////////////////**************************************///////////////////////////
/////////////////////////////////////////////////MAIN PROGRAM//////////////////////////////////////////
//////////////////////////////////////**************************************///////////////////////////
//////////////////////////////////////**************************************///////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

 
void main() {

  TRISB=0;  // port B en sortie
  PORTB=0b0000000000000000;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////*****************************//////////////////////////////////////
  //////////////////////////////////////***Configuration de L'UART***//////////////////////////////////////
  //////////////////////////////////////****************************///////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  UART1_Init(9600);  // initialiser la communication à 9600 buad
  Delay_ms(100); // pour que le module bluetooth se stabilise


  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////*****************************//////////////////////////////////////
  //////////////////////////////////////***Configuration de L'MPU***///////////////////////////////////////
  //////////////////////////////////////****************************///////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////*****************************************************//////////////////////////////////////
  //////////////////////***par défaut le MPU est en sleep mode sleep , alors il faut l'activer***//////////////////
  //////////////////////*****************************************************//////////////////////////////////////


  I2C1_Init(400000);// initialiser la communication I2C à 400KHZ
  I2C1_Start();     // commencer le start bit
  I2C1_Write(0x68); // 0x68 adresse en hexadécimal de notre MPU
  I2C1_Write(0x6B); // on veut écrire sur le rgistre PWR_MGMT_1 (6B hex)
  I2C1_Write(0x00); // atver le gyro
  I2C1_Stop();      // arréter le signal


  ///////////////////////////////*****************************************************////////////////////////////
  ///////////////////////////////***On ajout le filtrage pour améliorer les données***////////////////////////////
  ///////////////////////////////*****************************************************////////////////////////////

  I2C1_Start();
  I2C1_Write(0x68);
  I2C1_Write(0x1A); //on veut écrire dans le registre CONFIG (1A hex)
  I2C1_Write(0x03); //on met 3 dans  DLPF_CFG[2:0] pour avoir un filtre digital passe bas de ~44KHz pour Accelerometer et ~42KHz pour le Gyroscope
  I2C1_Stop();



  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////*****************************//////////////////////////////////////
  //////////////////////////////////////***Configuration du timer 1***//////////////////////////////////////
  //////////////////////////////////////****************************///////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  TCS_bit=0;  // source interne
  TGATE_bit=0;
  TMR1=0;
  TSYNC_bit=0;
  TCKPS_0_bit=0;
  TCKPS_1_bit=0;
  PR1=65535;
  TON_bit=1;

  //// configuration de l'interruption du timer 1 ////
  T1IF_bit=0;  ///  mise a zero du flag
  T1IE_bit=1;
   
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////*****************************//////////////////////////////////////
  //////////////////////////////////////***Configuration du PWM***//////////////////////////////////////
  //////////////////////////////////////****************************///////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  // PWM 1
  OC1CONbits.OCM = 0b110; // Selectionner le mode "PWM mode without fault protection"
  OC1RS =0; // le rapport cyclique correspondant à la vitesse du moteur à guache
  OC1CONbits.OCTSEL = 0; // on selectione le TIMER2 comme base du temps du "compare 1"
  OC1R = 100; // la valeur initiale de ce registre n'est pas importante pour nous ('read_only register'),
              //car par la suite c'est la valeur de OC1RS qui sera copié dans OC1R(datasheet)
  // configuration Timer2

  T2CONbits.TON = 1; // activer le timer
  T2CONbits.TCS = 0; // sélectionner l'horloge interne
  T2CONbits.TGATE = 0; //  la mise à 0 de TGATE
  T2CONbits.TCKPS = 0b00; // la valeur du prescaler est 1
  TMR2 = 0x00; // remise à 0 du timer 2
  PR2 = 49999; // on choisit une période de ~10ms inférieur à celle du timer1 (~20ms) pour que la valeur ajusté du duty_cycle
               //sera prise en compte ( la période est calculé par : PWM period = [(PR2)+1]*Tcy*(TMR2 prescaler value)  )
  IEC0bits.T2IE = 1; // activer l'interruption du timer2

  // PWM 2

  OC2CONbits.OCM = 0b110; // Selectionner le mode "PWM mode without fault protection"
  OC2RS =0; // le rapport cyclique correspondant à la vitesse du moteur à droite
  OC2CONbits.OCTSEL = 1; //  on selectione le TIMER3 comme base du temps du "compare 2"
  OC2R = 200; // la valeur initiale de ce registre n'est pas importante pour nous ('read_only register'),
              //car par la suite c'est la valeur de OC2RS qui sera copié dans OC2R


  // Initialize and enable Timer3
  T3CONbits.TON = 1; // activer le timer
  T3CONbits.TCS = 0; // sélectionner l'horloge interne
  T3CONbits.TGATE = 0; //  la mise à 0 de TGATE
  T3CONbits.TCKPS = 0b00; // la valeur du prescaler est 1
  TMR3 = 0x00; // remise à 0 du timer 3
  PR3 = 49999; // meme chose , une periode de ~10ms
  IFS0bits.T3IF = 0; // mise à 0 du flag
  IEC0bits.T3IE = 1; // activer l'interruption du timer3


  RPOR0bits.RP1R= 0b10010;             // 10011 pour OC2  et 10010 pour OC1
  RPOR0bits.RP0R= 0b10011;

  RB10_bit=1;
  //RB14_bit=1;
  RB11_bit=0;

  while(1)
  { 
   
    if (UART_Data_Ready())
    {  
        // If data is received,
        mot_recu = UART_Read();     // read the received data,
        mot_compteur=0;
    }

    if(mot_compteur <= 100) {mot_compteur++;} // le mot recu sera valide pour  100 loop
    else mot_recu=0x00;  // Aprés 100 loop si l'utilisateur n'envoie rien , le mot sera effacé pour arréter le robot


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////*************************************************//////////////////////////////////////
  //////////////////////////////////////LE CALCUL DE L'ANGLE ACTUEL DU ROBOT PAR MPU6050//////////////////////////////////////
  //////////////////////////////////////************************************************//////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////********************************************/////////////////////////////////////
  //////////////////////////////////////Récupération des valeurs de l'accéléromètre//////////////////////////////////////
  //////////////////////////////////////*******************************************//////////////////////////////////////

    Acc_rawX=acc_x();
    Acc_rawY=acc_y();
    Acc_rawZ=acc_z();

    //////////////convertir les données d'accéléromètre aux angles//////////////

    Angle_acc = atan((Acc_rawY/16384.0)/sqrt(pow((Acc_rawX/16384.0),2) + pow((Acc_rawZ/16384.0),2)))*rad_to_deg;
    //l'accéléromètre est par défaut sur l mode ±2g ce qui donne une sensibilté de 16384 /g , d'ou de Acc_rawY par 16384
    // et cette est en radian alors on multiplie par 180/pi pour avoir la valeur en degrée


  //////////////////////////////////////**************************************/////////////////////////////////////
  //////////////////////////////////////Récupération des valeurs du gyroscope//////////////////////////////////////
  //////////////////////////////////////**************************************/////////////////////////////////////

    Gyr_rawX=gyro_x();

    //////////////convertir les données du gyro aux angles//////////////

    Angle_gyro = Gyr_rawX/131.0; // ici on divise par 131 le gyro est par défaut configuer sur la valeur ± 250 °/s et la sortie correspondante
                                    //à cette valeur est 131 pour chaque 1°/s  d'ou la division (Sur datasheet)

  //////////////////////////////////////**************************************************************//////////////////////////////////////
  //////////////////////////////////////COMBINER LES DEUX ANGLES EN UTILISANT LE FILTRE COMPLEMENTAIRE//////////////////////////////////////
  //////////////////////////////////////**************************************************************//////////////////////////////////////

    angle_totale = 0.98 *(angle_totale + Angle_gyro*delta_t) + 0.02*Angle_acc;



  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////*************************************************//////////////////////////////////////
  //////////////////////////////////////**************LA REGULATION PID*****************/////////////////////////////////////////////////////////////////////
  //////////////////////////////////////************************************************//////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    //////////////angle_totale est l'angle  PITCH qu'on veut//////////////

    erreur = angle_totale  - angle_desire; //Calucle de l'erreur
        
    ///////////////////////ERREUR PROPORTIONNELLE (présent)//////////////
    pid_p = kp*erreur;
    ///////////////////////ERREUR INTEGRALE (passé)/////////////////
        
    pid_i = pid_i+(ki*erreur);

    if(pid_i > 400) {pid_i = 400;}                              //Limiter l'accumulation d'erreur
    else if(pid_i < -400) {pid_i= -400;}

    ///////////////////////ERREUR DIFFERENTIELLE (futur) //////////////
        
    pid_d = kd*((erreur - erreur_prec)/delta_t); ///
    ///////////////////////PID TOTAL/////////////////
    PID = pid_p + pid_d+pid_i;
    ///////////////////////sauvegarder l'erreur////////
    erreur_prec = erreur;

    vitesse = fabs(PID);   // PID peut étre négative alors il faut prendre sa valeur absolue pour commander les moteurs
    vitesse_moteur_g=vitesse;
    vitesse_moteur_d=vitesse;

    if(vitesse< 5 && vitesse > -5) {vitesse = 0;} // une sortie de vitesse =5 indique que le robot est à son équilibre alors on arrete
                                                   //les moteurs pour éliminer les vibrations

    if(angle_totale > 40 || angle_totale < -40)
      {  
        vitesse = 0;   // si le robot dépasse cette angle càd qu'il est tombé alors on arréte les moteurs
        pid_i=0;       //puiseque la partie integral du PID se souvient du passé alors il faut la mettre à 0 pour ne pas avoir une sortie
      }
   
    
  //////////////////////////////////////**********************************//////////////////////////////////////
  //////////////////////////////////////le controll du robot par bluetooth//////////////////////////////////////
  //////////////////////////////////////**********************************//////////////////////////////////////



  if(mot_recu & 0x0001)
  {                     // si le premier bit du mot est à 1 tourner le robot à gauche
      vitesse_moteur_g += vitesse_rot;       //incrémenter la vitsse du moteur droite par 30 (définit en haut)
      vitesse_moteur_d -= vitesse_rot;       //décrémenter la vitsse du moteur gauche par 30 (définit en haut)
  }

  if(mot_recu & 0x0002){                        // si le deuxième bit du mot est à 1 tourner le robot à droite
    vitesse_moteur_g -= vitesse_rot;
    vitesse_moteur_d += vitesse_rot;
  }

  if(mot_recu & 0x0003)
  {                        // si le troisième bit du mot est à 1 faire avancer le robot
   if(angle_desire > -2.5)angle_desire -= 0.05;
   if(vitesse > vitesse_max)angle_desire -= 0.005;
  }

  if(mot_recu & 0x0004)
  {                        // si le quatrième bit du mot est = 1 faire reculer le robot
   if(angle_desire < 2.5)angle_desire += 0.05;
   if(vitesse <  vitesse_max)angle_desire += 0.005;
  }
 }
}
