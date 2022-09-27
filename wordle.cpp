#include <iostream>
#include <string>
#include <algorithm>
#include <ctype.h>

using namespace std;

// funkcija kura skaita cik burt ir varda vai cik burti ir varda lidz konretai vietai
int cikBurti(char burts, string vards, int idx = 0, bool skaititCikBija = false){
    int skaits = 0;
    int garums = skaititCikBija == true ? idx + 1 : 5;
    for(int i = 0; i < garums; i++){
        if(vards[i] == burts){
            skaits++;
        }
    }
    return skaits;
}

// funkcija kura atbild par burtu iekrasosanu
string iekrasot(string ievade, string pareizs){
    string krasuIzvade, krasainsBurts;
    bool irVarda = false;
    string pareizs2 = pareizs; // string no kura bus iznemti lietoti burti lai vini velak netrauce
    string vardsMas[5];
    
    // cikls kurs iekrasos visus pareizi saliktos burtus
    for(int i = 0; i < 5; i++){
        ievade[i] = (int)ievade[i] < 97 ? char((int)ievade[i] + 32) : ievade[i]; // parvērš lielos burtus uz maziem
        if(ievade[i] == pareizs[i]){
            pareizs2[i] = '-';  // atzīmē un vēlak izņem 
            krasainsBurts = "\033[32m";
            krasainsBurts += ievade[i];
            krasainsBurts += "\033[0m";
            vardsMas[i] = krasainsBurts;
        }
    }
    pareizs2.erase(remove(pareizs2.begin(), pareizs2.end(), '-'), pareizs2.end()); // līnija panemta no https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string
    for(int x = 0; x < 5; x++){
        irVarda = false; 
        for(int idx = 0; idx < 5; idx++){
            if(ievade[x] == pareizs2[idx]) irVarda = true;
        }
        // iekraso burtu ar dzeltenu vai neiekraso vispar
        if(vardsMas[x] == ""){ // parbauda vai masiva vieta nav aiznemta
            if(cikBurti(ievade[x], ievade, x, true) <= cikBurti(pareizs[x], pareizs) && irVarda){ // parbauda vai vienadi burti nebija atkartojusies parak daudz
                krasainsBurts = "\033[33m";
                krasainsBurts += ievade[x];
                krasainsBurts += "\033[0m";
                vardsMas[x] = krasainsBurts;
            }else{
                krasainsBurts = ievade[x];
                vardsMas[x] = krasainsBurts;
            }
        }
        krasuIzvade += vardsMas[x]; // ieliek masiva elementu izvades string
    }
    return krasuIzvade;
}

int main() {
    
    // mainigo ieviessana
    start:
    string minejums[6], teksts;
    int pointer, counter = 0;
    bool skip = true, win = false;
    char turpinat;
    string vardi[] = {
        "ausma", "asara", "balss", "burka", "celis", "ciems", "darbs", "diena", "ezers", "gaiss", "griki", "horda",
        "iesms", "josta", "kakls", "kauls", "laime", "lacis", "maize", "metrs", "nakts", "nulle", "ozols", "opera",
        "pelni", "plava", "risks", "rudzi", "saite", "sargs", "talka", "tikls", "uguns", "velns", "video", "zieds"
        
    };
    srand(time(NULL));
    int varduSkaits = sizeof(vardi) / sizeof(vardi[0]);
    int index = rand() % varduSkaits;
    string vards = vardi[index];
    
    // speles paskaidrojums
    cout << "Šī programma ir spēles 'Wordle' pārtaisīšana C++ programmēšanas valodā\n\n";
    cout << "Spēles noteikumi ir šadi:";
    cout << "\n\u2022Jums ir doti 6 mēginājumi lai uzminēt 5 burtu vārdu;";
    cout << "\n\u2022Ja burts ir \033[32mzaļš\033[0m, tad tas ir attiecīgajā vietā;";
    cout << "\n\u2022Ja burts ir \033[33mdzeltens\033[0m, tad tas ir minētajā vārdā, bet neatrodas pareizajā vietā.";
    cout << "\n\n(Spiežat Enter lai sākt spēli)";
    cin.ignore(); // līnija panemta no no https://stackoverflow.com/questions/42818899/detecting-enter-key-in-c
    cout << "\033c";

    for(counter; counter < 6; counter++){ // visu meginajumu cikls
        cout << "\e[1m\tWordle\e[0m\n\n";
        for(int i = 0; i < 6; i++){ // cikls kurs izvada visus vajadzigos elemetus uz ekrana
            if(minejums[i] != ""){
                cout << "\t" << iekrasot(minejums[i], vards);
            }else if(minejums[i] == "" && skip){
                pointer = i; // saglaba vietu kur vajadzes taisit ievadi,
                skip = false;// ka ari uztaisa mainigo kas stradas ka idex pedejai ievaditajai vertibai masiva
            }else{
                cout << "\t";
                for(int x = 0; x < 5; x++) cout << "\u25A2"; // izvada kvadratus
            }
            cout << "\n";
            if(i == 5 && minejums[i] == ""){ // atgriezas atpakaļ uz pointeri lai izdarit ievadi
                for(int low = 5; low >= pointer; low--) cout << "\x1b[A\r";
                cout << "\t";
                cin >> minejums[pointer];
                cout << "\033c";
            }else if(i == 5 && minejums[i] != ""){
                cout << "\033c";
            }
        }
        skip = true;
        for(int x = 0; x < 5; x++){
            if(!isalpha(minejums[pointer][x])){ // funkcija no https://www.tutorialspoint.com/isalpha-and-isdigit-in-c-cplusplus
                minejums[pointer] = "";
                counter--;
                break;
            }
        }
        if(minejums[pointer].length() != 5){
            minejums[pointer] = "";
            counter--;
        } 
        if(minejums[pointer] == vards){
            win = true;
            break;
        }
    }
    
    teksts = win ? "\nApsveicu!\nJūs pareizi atminējāt iegādāto vārdu!" : "\nDiemžēl jūs neatminējāt iegādāto vārdu,\nPareizais vārds bija " + vards;
    
    cout << "\e[1m\tWordle\e[0m\n\n";
    for(int i = 0; i < 6; i++){
        if(minejums[i] != ""){
            cout << "\t" << iekrasot(minejums[i], vards) << "\n";
        }else{
            cout << "\t";
            for(int x = 0; x < 5; x++) cout << "\u25A2";
            cout << "\n";
        }
    } 
    cout << teksts;
    
    cout << "\n\nMēģināt vēlreiz? (j/n): ";
    cin >> turpinat;
    if(turpinat == 'j' || turpinat == 'J' || turpinat == 'y' || turpinat == 'Y'){
        cout << "\033c";
        goto start;
    }
    

  return 0;
}