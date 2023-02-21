#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_LENGTH = 50; //Maximale Länge des Levels
const int MAX_WIDTH = 100; //Maximale Breite des Levels

struct Player {
    int x, y;   //Spielerposition
    int stage;  //Derzeitige Stage
    int falls;  //Anzahl an Stürz-Feldern

    Player(int startX, int startY, int startStage, int startFalls) {
        x = startX;
        y = startY;
        stage = startStage;
        falls = startFalls;
    }
};

void loadLevel(string dateiname, char level[MAX_LENGTH][MAX_WIDTH], int& length, int& width, Player& player) {
    ifstream file(dateiname);
        if (!file) { //Dateifehler (!file) resultiert in Fehlermeldung
            cerr << "Fehler: Datei " << dateiname << " konnte nicht geöffnet werden." << endl; 
        exit(1);
    }

   file >> length >> width;
   file.ignore();  //ignoriere Zeilenumbruch nach Breite

    //Level wird aus Textdatei eingelesen
    for (int i = 0; i < length; i++) {
        file.getline(level[i], MAX_WIDTH);
        for (int j = 0; j < width; j++) {
            if (level[i][j] == 'S') {
                //"S" wird als Startsymbol gefunden und der Spieler wird dort platziert
                player.x = j;
                player.y = i - 2;
                player.stage = 0;
                player.falls = 0;
                level[i][j] = ' ';
            } //"O" wird als Ziel gefunden
            else if (level[i][j] == 'O') {
                level[i][j] = '-';
                level[i - 2][j] = '-';
                level[i - 4][j] = 'O';
            }
        } 
    }
}

void printLevel(char level[MAX_LENGTH][MAX_WIDTH], int length, int width, Player player) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            if (i == player.y && j == player.x) {
                //Spielfigur wird gezeichnet
                cout << " o " << endl;
                cout << "/|\\" << endl;
                cout << "/ \\" << endl;
                j += 2;
            }
            else {
                cout << " " << level[i][j] << " ";
            }
        }
        cout << endl;
    }
}
//boolean benutzt, um Bedingungen zu überprüfen
bool isLadder(char c) { //Leiter wird deklariert
    return c == 'H';
}

bool isGoal(char c) {  //Ziel wird deklariert
    return c == 'O';
}

bool isFree(char c) {  //Freie Position wird deklariert
    return c == ' ';
}
bool isStart(char c) { //Startposition wird deklariert
    return c == 'S';
}

void BewegungLinks(char level[MAX_LENGTH][MAX_WIDTH], int length, int width, Player& player) {
    //Es wird links auf einen freien Platz überprüft
    if (player.x > 0 && isFree(level[player.y][player.x - 1])) { //Falls frei ist, bewegt sich die Figur eine Position nach links auf der X-Achse
        player.x--;
    }
    else if (player.x > 0 && isLadder(level[player.y][player.x - 1])) {
        //Es wird links nach einer Leiter gesucht
        for (int i = player.y - 1; i >= 0; i--) {
            if (isLadder(level[i][player.x - 1])) { //Falls eine Leiter gefunden wurde, bewegt sich die Figur auf der Leiter nach unten
                player.y = i;
                break;
            }
        }
        player.x--;
    }
}


void BewegungRechts(char level[MAX_LENGTH][MAX_WIDTH], int length, int width, Player& player) {
    //Es wird rechts auf einen freien Platz überprüft
    if (player.x < width - 1 && isFree(level[player.y][player.x + 1])) { //Falls frei ist, bewegt sich die Figur eine Position nach rechts auf der X-Achse
        player.x++;
    }
    else if (player.x < width - 1 && isLadder(level[player.y][player.x + 1])) {
        //Es wird rechts nach einer Leiter gesucht
         for (int i = player.y - 1; i >= 0; i--) {
             if (isLadder(level[i][player.x + 1])) { //Falls eine Leiter gefunden wurde, bewegt sich die Figur auf der Leiter nach unten
                 player.y = i;
                 break;               
            }            
        }
         player.x++;  
    } 
}

 bool hasGround(char level[MAX_LENGTH][MAX_WIDTH], int length, int width, Player player) {
     //Es wird unter dem Spieler überprüft, ob ein Boden erkannt werden kann
         for (int i = player.y + 1; i < length; i++) {
         if (level[i][player.x] == '-' || isLadder(level[i][player.x])) {
             return true;
        }
        else if (isGoal(level[i][player.x])) {
             //Ziel erreicht - Spiel gewonnen
                 cout << "Glückwunsch, du hast gewonnen!" << endl;
             return false;
        }        
    }
     return false;   
}

 void runGame(char level[MAX_LENGTH][MAX_WIDTH], int length, int width, Player & player) {
     printLevel(level, length, width, player); //Level wird in runGame gezeichnet
    
     while (true) { //solange Bedingung erfüllt wird, wird char "Bewegung" angezeigt
         char Bewegung; 
         cout << "Bewege dich mit den folgenden Buchstaben (a/d/s): ";
         cin >> Bewegung;
        //switch case für verschiedene Verzweigungen/Möglichkeiten
             switch (Bewegung) {
             case 'a':   //Nach Links bewegen
                 BewegungLinks(level, length, width, player);
                 break;
             case 'd':   //Nach Rechts bewegen
                 BewegungRechts(level, length, width, player);
                 break;
             case 's':   //Die Leiter benutzen
                 if (isLadder(level[player.y][player.x])) {                
                         player.y--;  
                }
                else if (hasGround(level, length, width, player)) {
                     //Falls Boden verfügbar, spring zur nächsten Etage
                        ; player.y += 2;
                     player.stage++;
                   
                     if (player.stage == length / 2 - 1) {
                         //Ziel erreicht - Spiel gewonnen
                             cout << "Glueckwunsch, du hast gewonnen!" << endl;
                         return;                      
                    }                   
                }
                            break;               
            }       
             if (!hasGround(level, length, width, player)) { //Überprüft, ob ein Boden unter der Figur existiert
             player.falls++;                                //Falls kein Boden vorhanden ist, fällt der Spieler um eine Position auf der Y-Achse
             if (player.falls > 5 || player.stage == 0) { //Das Spiel ist durch eine der vorgegebenen Bedingungen vorbei
                     cout << "Game Over, du bist runtergefallen!" << endl;
                 return;       
            }
             //Der Spieler wird auf einer niedrigeren Stage respawned
                 for (int i = player.stage; i >= 0; i--) {
                 for (int j = 0; j < width; j++)
                     if (isStart(level[i][j])) {
                     player.x = j;
                     player.y = i - 2;
                     player.stage = i - 2;
                     break;                   
                 }                
             }           
         }      
             system("cls"); //Konsole zurücksetzen und geupdatetes Level anzeigen
         printLevel(level, length, width, player);
     }   
}

 int main() {
     string dateiname;  //Textdatei wird zur Verfügung gestellt
     cout << "Gib den Dateinamen ein: ";
     cin >> dateiname;

         ifstream file(dateiname); 
    
         if (!file.is_open()) { //Falls die Datei nicht existiert, wird eine Fehlermeldung angezeigt
             cout << "Der Dateiname " << dateiname << " existiert nicht!" << endl;
             return 0;       
         }   
   
        int length, width;
        file >> length >> width;
    
         char level[MAX_LENGTH][MAX_WIDTH];
    
         for (int i = 0; i < length; i++) {
         for (int j = 0; j < width; j++) {
             file >> level[i][j];            
        }        
    }   
         file.close();   
         //Finde die Start Position
         int startX = 0, startY = 0;
     for (int i = 0; i < length; i++) {
         for (int j = 0; j < width; j++) {
             if (isStart(level[i][j])) {
                startX = j;
                startY = i - 2;
                 break;                
            }          
        }        
    }    
     
     Player player(startX, startY, 0, 0);
        loadLevel("level.txt", level, length, width, player); //Level nach der Vorlage laden      
         runGame(level, length, width, player); //Lässt das Spiel starten
         return 0;   
}


/*
* Bitte beachten Sie bei der Gestaltung des Levels, die Länge und Breite gemäß dem selbst gestalteten Level anzupassen. Wie bereits erwähnt, muss für die erfolgreiche
* Einlesung, das Level vollständig mit den Zeichen "H", "O", "-", "S" und " " gestaltet werden. Außerdem sollten die "leeren" und unbenutzten Felder, welche sich rechts
* von den gesetzten Zeichen für das Spielfeld befinden, mit " " (Leerzeichen) gefüllt werden, um eine akkurate Visualisierung des Levels zu ermöglichen.
* 
* Die Ausführung meines Programmes funktioniert nicht vollständig, da ich vor dem Problem stand, das Level und den Spieler unabhängig voneinander funktionieren zu lassen.
* Dadurch entstanden Fehler bei der Bewegung, sowie bei der visuellen Ordnung des Levels. 
* 
*/