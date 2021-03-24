#include <IRremote.h>

const char DIN_RECEPTEUR_INFRAROUGE = 2;
IRrecv monRecepteurInfraRouge (DIN_RECEPTEUR_INFRAROUGE);
decode_results messageRecu;

const int led = 12;
const int ledOn = 11;
const int ledBleu = 7, ledBlanc = 6, ledRouge = 5;

const long On_Off = 0xFFA25D;

const long Bt0 = 0xFF6897;
const long Bt1 = 0xFF30CF;
const long B2 = 0xFF18E7;
const long B3 = 0xFF7A85;
const long B4 = 0xFF10EF;
const long B5 = 0xFF38C7;
const long B6 = 0xFF5AA5;
const long B7 = 0xFF42BD;
const long B8 = 0xFF4AB5;
const long B9 = 0xFF52AD;

const long VolPlus = 0xFF629D;
const long VolMoins = 0xFFA857;

const long Precedent = 0xFF22DD;
const long Play_Pause = 0xFF02FD;
const long Suivant = 0xFFC23D;

const long Haut = 0xFFE01F;
const long Bas = 0xFF906F;

const long Func_Stop = 0xFFE21D;
const long EQ = 0xFF9867;
const long ST_REPT = 0xFFB04F;

void setup()
{
    Serial.begin(9600);

    monRecepteurInfraRouge.enableIRIn();
    monRecepteurInfraRouge.blink13(true);

    pinMode(led, OUTPUT);

    pinMode(ledOn, OUTPUT);
    digitalWrite(ledOn, HIGH);
    
    Serial.println("Bouton On / Off : permet de Deverouiller / Verouiller la telecommande (quand verrouilé, la led roug s'allume");
    Serial.println("Func Stop : Pour tous les numéros qui vont suivre, permet de sortir du mode proprement");
    Serial.println("Choisi le mode");
    Serial.println("0 : Eteint tout");
    Serial.println("1 : Allume tout");
    Serial.println("2 : Clignotte la led bleu (plus bouton +/- pour accelerer / diminuer la vitesse des leds");
    Serial.println("3 : Allume la led bleu");
    Serial.println("4 : Allume Bleu Blanc Rouge");
    Serial.println("5 : Choisis un nombre en faisant precedent / suivant (en utilisant la voie serie on voit le nombre choisi) puis appuis sur Play Pause et ton nombre choisis sera le nombre de seconde ou la led s'allumera  ");
    Serial.println();
    Serial.println();
}

boolean allume = 0;
boolean clignotte = 0;
boolean choisisPuisAppuis = 0;
boolean chenillard = 0;
int pause;
int nombreDeRepet;


void loop()
{
    if (monRecepteurInfraRouge.decode(&messageRecu))
    {
        if (messageRecu.value == On_Off)
        {
            if (allume == 0)
            {
                allume = 1;
                digitalWrite(ledOn, LOW);
            }
            else
            {
                allume = 0;
                EteintTout();
                digitalWrite(ledOn, HIGH);
            }
        }
        
        if (allume == 1)
        {
            if(messageRecu.value == Bt0)
            {
                EteintTout();
            }
            if (messageRecu.value == Bt1)
            {
                digitalWrite(led, HIGH);
                digitalWrite(ledBleu, HIGH);
                digitalWrite(ledBlanc, HIGH);
                digitalWrite(ledRouge, HIGH);
            }
            if (messageRecu.value == B2)
            {
                clignotte = 1;
                pause = 1000;
            }
            BoutonStop();
            if (messageRecu.value == B3)
            {
                digitalWrite(led, HIGH);
            }
            if (messageRecu.value == B4)
            {
                digitalWrite(ledBleu, HIGH);
                digitalWrite(ledBlanc, HIGH);
                digitalWrite(ledRouge, HIGH);
            }
            if (messageRecu.value == B5)
            {
                choisisPuisAppuis = 1;
                nombreDeRepet = 0;
            }
            if (messageRecu.value == B6)
            {
                chenillard = 1;
            }
                     
            
        }
        delay(1);
        monRecepteurInfraRouge.resume();
    }

    while (clignotte == 1)
    {
        digitalWrite(led, HIGH);
        VerifTelec();
        delay(pause);
        VerifTelec();
        digitalWrite(led, LOW);
        VerifTelec();
        delay(pause);
        VerifTelec();
    }

    while (choisisPuisAppuis == 1)
    {
        if (monRecepteurInfraRouge.decode(&messageRecu))
        {
            if (messageRecu.value == Suivant)
            {
                nombreDeRepet = nombreDeRepet + 1;
                Serial.println(nombreDeRepet);
            }
            if (messageRecu.value == Precedent)
            {
                nombreDeRepet = nombreDeRepet - 1;
                Serial.println(nombreDeRepet);
            }
            
            if (messageRecu.value == Play_Pause)
            {
                for(int i=1; i<=nombreDeRepet; i++)
                {
                    digitalWrite(led, HIGH);
                    delay(1000);
                }
                digitalWrite(led, LOW);
                nombreDeRepet = 0;
            }
            BoutonStop();
            
            

            delay(1);
            monRecepteurInfraRouge.resume();
        }
    }

    while (chenillard == 1)
    {
        digitalWrite(ledOn, HIGH);
        delay(500);
        digitalWrite(led, HIGH);
        delay(500);
        digitalWrite(ledBleu, HIGH);
        delay(500);
        digitalWrite(ledBlanc, HIGH);
        delay(500);
        digitalWrite(ledRouge, HIGH);
        delay(500);
        EteintTout();
        
        if (monRecepteurInfraRouge.decode(&messageRecu))
        {
            BoutonStop();

            delay(1);
            monRecepteurInfraRouge.resume();
        }


    }
    
    
}

void VerifTelec()
{
    if (monRecepteurInfraRouge.decode(&messageRecu))
    {
        BoutonStop();
        
        if (messageRecu.value == VolPlus)
        {
            pause = pause + 1000;
        }
        if ((messageRecu.value == VolMoins) && (pause != 1000))
        {
            pause = pause - 1000;
        }
        
        delay(1);
        monRecepteurInfraRouge.resume();
    }
}

void BoutonStop()
{
    if(messageRecu.value == Func_Stop)
        {
            choisisPuisAppuis = 0;
            clignotte = 0;
            chenillard = 0;
            EteintTout();
        }
}

void EteintTout()
{
    digitalWrite(led, LOW);
    digitalWrite(ledOn, LOW);
    digitalWrite(ledBleu, LOW);
    digitalWrite(ledBlanc, LOW);
    digitalWrite(ledRouge, LOW);
}
