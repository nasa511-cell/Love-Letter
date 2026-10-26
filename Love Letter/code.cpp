/*
情书：每人手里有一张卡牌，每次行动时摸一张卡牌，并选择一张卡牌打出
0.守卫 guard 猜测另外一位玩家的手牌 5张
1.祭司 priest 查看另一位玩家的手牌 2张
2.男爵 baron 私下比较手牌点数 2张
3.侍女 handmaid 无法选中 2张
4.王子 prince 强迫一位玩家弃掉手牌，并从牌堆中摸一张 2张
5.国王 king 与另一位玩家交换手牌 1张
6.女伯爵 countess 有王子或国王时会强制打出 1张
7.公主 princess 打出时会立即出局 1张
*/


#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
using namespace std;

//全局变量：卡牌数组
string card_name[] = {"守卫", "祭司", "男爵", "侍女", "王子", "国王", 
                      "女伯爵" ,"公主"}; 

//开始菜单函数
void start_menu(){
    cout << "欢迎来到情书" << endl;
    cout << "每人手里有一张卡牌，每次行动时摸一张卡牌，并选择一张卡牌打出" << endl;
    cout << "所有的手牌种类如下，数字为卡牌代表的点数：" << endl;   
    cout << "当所有手牌全部被摸完，存活的玩家中点数最大的获得胜利" << endl;
    cout << "0.守卫 guard：猜测另外一位玩家的手牌，不能猜测守卫，猜中对方出局" << endl;
    cout << "1.祭司 priest：查看另一位玩家的手牌" << endl;
    cout << "2.男爵 baron：私下比较手牌点数，点数小的一方出局" << endl;
    cout << "3.侍女 handmaid：不能成为其它玩家的技能目标，直到自己的下个回合" << endl;
    cout << "4.王子 prince：强迫一位玩家弃掉手牌，并从牌堆中摸一张" << endl;
    cout << "5.国王 king：与另一位玩家交换手牌" << endl;
    cout << "6.女伯爵 countess：手牌中有王子或国王时会强制打出女伯爵" << endl;
    cout << "7.公主 princess：打出公主的玩家立刻死亡" << endl;
    cout << "场上有四名玩家，分别是你（编号1），玩家2，玩家3，玩家4" << endl;
 
}

//统一的输入函数，防止输入字母导致死循环
int read_int(const string& prompt){
    int x;
    while(true){
        cout << prompt;
        if (cin >> x) {return x;}
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "输入失败，请重新输入！" << endl;
    }
}

//随机抽取一张牌
int create_card(vector<int>& rest, int& count){
    if (count == 0){ return -1; }
    int x = -1;
    while(x < 0 || rest[x] == 0){
        x = rand() % 8;

    }
    rest[x]--;
    return x;

}

class Player{
public:
    vector<int> card; //手牌
    bool isAI; //是不是玩家
    bool isAlive; //是否存活
    bool bechosen; //是否可选中

    //构造函数
    Player(int id, bool isAI, bool isAlive = true, bool bechosen = true): 
            card({id}), isAI(isAI), isAlive(isAlive), bechosen(bechosen) {}

    //弃牌,并返回是否造成死亡
    bool throw_card(vector<int>& rest, int& count){
        if (card[0] == 7){
            isAlive = false;
            return true;
        }
        int c = create_card(rest, count);
        if (c == -1) {return false;}
        count--;
        card[0] = c;
        return false;

    }

    //玩家回合
    void player_turn(vector<int>& rest, vector<Player>& member, int& c, int& count){
        cout << "你的回合!" << endl;
        cout << "你摸到了" << c << "." << card_name[c];
        cout << "，你原来的手牌是" << card[0] << "." << card_name[card[0]] << endl;
        int sum = card[0] + c;
        //女伯爵强制打出
        if ((c == 6 && (card[0] == 4 || card[0] == 5))
            || (card[0] == 6 && (c == 4 || c == 5))){
            sleep(1);
            cout << "你必须打出6.女伯爵" << endl;
            card[0] = sum - 6;
            return;

        }
        int show = 0;
        do{
            show = read_int((string)"请选择你要出的牌: ");
            if (show != c && show != card[0]){
                cout << "你没有这张牌！" << endl;
            }
            else{
                break;
            }
        } while (true);
        cout << "你打出了" << show << "." << card_name[show] << endl;
        card[0] = sum - show;

        switch(show){
            case 0:{
                int choose = 0;
                do{
                    choose = read_int((string)"请选择你要猜测身份的玩家编号：");
                    if (choose < 1 || choose > 4 || !member[choose - 1].bechosen
                       || !member[choose - 1].isAlive){
                        cout << "选择错误，请重新选择！" << endl;
                       }
                    else{
                        break;
                    }
                } while(true);

                if (choose == 1){
                    cout << "你选择了自己作为技能目标" << endl;
                    cout << "你的回合结束" << endl;
                    return;
                }
                else{
                    int guess = 0;
                    do{
                        guess = read_int((string)"请猜测他的身份牌：");
                        cin >> guess;
                        if (guess < 0 || guess > 7){
                            cout << "请重新猜测!" << endl;
                        }
                        else{
                            break;
                        }
                    } while (true);

                    if (member[choose - 1].card[0] == guess){
                        cout << "猜测正确！" << endl;
                        cout << "player" << choose << "死亡！" << endl;
                        cout << "你的回合结束" << endl;
                        member[choose - 1].isAlive = false;
                    }
                    else{
                        cout << "猜测失败！" << endl;
                        cout << "你的回合结束" << endl;
                    }
                }
                break;
            }

            case 1:{
                int choose = 0;
                do{
                    choose = read_int((string)"请选择你要查看身份的玩家编号：");
                    if (choose < 1 || choose > 4 || !member[choose - 1].bechosen
                       || !member[choose - 1].isAlive){
                        cout << "选择错误，请重新选择！" << endl;
                       }
                    else{
                        break;
                    }
                } while(true);

                if (choose == 1){
                    cout << "你选择了自己作为技能目标" << endl;
                    cout << "你的回合结束" << endl;
                    return;
                }
                else{
                    cout << "它的身份是" << member[choose - 1].card[0] << ".";
                    cout << card_name[member[choose - 1].card[0]] << endl;
                    cout << "你的回合结束" << endl;
                }
                break;

            }

            case 2:{
                int choose = 0;
                do{
                    choose = read_int((string)"请选择你要比较手牌的玩家编号：");
                    if (choose < 1 || choose > 4 || !member[choose - 1].bechosen
                       || !member[choose - 1].isAlive){
                        cout << "选择错误，请重新选择！" << endl;
                       }
                    else{
                        break;
                    }
                } while(true);

                if (choose == 1){
                    cout << "你选择了自己作为技能目标" << endl;
                    cout << "你的回合结束" << endl;
                    return;
                }
                else{
                    int other = member[choose - 1].card[0];
                    cout << "它的身份是" << other << ".";
                    cout << card_name[other] << endl;
                    if (card[0] > other){
                        cout << "它死亡" << endl;
                        member[choose - 1].isAlive = false;
                    }
                    else if (card[0] < other){
                        cout << "你死亡" << endl;
                        isAlive = false;
                    }
                    else{
                        cout << "你们点数相同" << endl;
                    }
                    cout << "你的回合结束" << endl;
                }
                break;

            }

            case 3:{
                cout << "你无法被选中，持续一回合" << endl;
                bechosen = false;
                break;

            }

            case 4:{
                int choose = 0;
                do{
                    choose = read_int((string)"请选择你要使其弃牌的玩家编号：");
                    if (choose < 1 || choose > 4 || !member[choose - 1].bechosen
                       || !member[choose - 1].isAlive){
                        cout << "选择错误，请重新选择！" << endl;
                       }
                    else{
                        break;
                    }
                } while(true);

                if (choose == 1){
                    cout << "你弃掉了" << card[0] << "." << card_name[card[0]] << endl;

                }

                bool someone_dead = member[choose - 1].throw_card(rest, count);
                if (someone_dead){
                    if (choose == 1){
                        cout << "你死亡" << endl;
                        return;
                    }
                    else{
                        cout << "它死亡" << endl;
                    }
                }

                if (choose == 1){
                    cout << "你获得了" << card[0] << "." << card_name[card[0]] << endl;
                }

                break;

            }

            case 5:{
                int choose = 0;
                do{
                    choose = read_int((string)"请选择你要使交换手牌的玩家编号：");
                    if (choose < 1 || choose > 4 || !member[choose - 1].bechosen
                       || !member[choose - 1].isAlive){
                        cout << "选择错误，请重新选择！" << endl;
                       }
                    else{
                        break;
                    }
                } while(true);

                if (choose == 1){
                    cout << "你选择了自己作为技能目标" << endl;
                    cout << "你的回合结束" << endl;
                    return;
                }
                else{
                    int other = member[choose - 1].card[0];
                    auto it = card[0];
                    card[0] = member[choose - 1].card[0];
                    member[choose - 1].card[0] = it;
                    cout << "你的身份变为" << card[0] << ".";
                    cout << card_name[card[0]] << endl;
                    
                }
                break;

            }

            case 6:{
                cout << "你弃掉了6.女伯爵" << endl;
                break;
            }

            case 7:{
                cout << "你弃掉了7.公主" << endl;
                cout << "你死亡" << endl;
                isAlive = false;
                break;
            }
        }
        
    }

    void AI_turn(vector<int>& rest, vector<Player>& member, int& cur_id, int& count){
        string name = "player" + (to_string)(cur_id);
        int sum = card[0] + card[1];
        //女伯爵强制打出
        if ((card[1] == 6 && (card[0] == 4 || card[0] == 5))
            || (card[0] == 6 && (card[1] == 4 || card[1] == 5))){
            cout << name << "打出6.女伯爵" << endl;
            card[0] = sum - 6;
            card.pop_back();
            return;

        }
        int show = card[rand() % 2];
        card.clear();
        card.push_back(sum - show);

        switch(show){
            case 0:{
                cout << name << "打出0.守卫" << endl;
                int choose = cur_id - 1;
                do{
                    choose = (choose + 1) % 4;
                }while(!member[choose].isAlive || !member[choose].bechosen);

                if (choose == cur_id - 1){
                    cout << name << "选择了自己作为技能目标" << endl;
                    cout << name << "的回合结束" << endl;
                    return;
                }
                else{
                    string tar = choose == 0 ? (string)"你" : ((string)"玩家" + (to_string)(cur_id));
                    int guess = rand() % 8;
                    cout << name << "猜测" << tar << "的身份为" << guess << endl;
                    if (member[choose].card[0] == guess){
                        cout << "猜测正确！" << endl;
                        member[choose].isAlive = false;
                    }
                    else{
                        cout << "猜测失败！" << endl;
                    }
                }
                break;
            }

            case 1:{
                cout << name << "打出1.祭司" << endl;
                /*
                int choose = 0;
                do{
                    cout << "请选择你要查看身份的玩家：";
                    cin >> choose;
                    if (choose < 1 || choose > 4 || !member[choose - 1].bechosen
                       || !member[choose - 1].isAlive){
                        cout << "选择错误，请重新选择！" << endl;
                       }
                    else{
                        break;
                    }
                } while(true);

                if (choose == 1){
                    cout << "你选择了自己作为技能目标" << endl;
                    cout << "你的回合结束" << endl;
                    return;
                }
                else{
                    cout << "它的身份是" << member[choose - 1].card[0] << ".";
                    cout << card_name[member[choose - 1].card[0]] << endl;
                    cout << "你的回合结束" << endl;
                }
                break;
                */
               cout << name << "选择了你查看手牌" << endl;
               break;

            }

            case 2:{
                cout << name << "打出2.男爵" << endl;
                int choose = cur_id - 1;
                do{
                    choose = (choose + 1) % 4;
                }while(!member[choose].isAlive || !member[choose].bechosen);

                if (choose == cur_id - 1){
                    cout << name << "选择了自己作为技能目标" << endl;
                    cout << name << "的回合结束" << endl;
                    return;
                }
                else{
                    string tar = choose == 0 ? (string)"你" : (string)"玩家" + (to_string)(choose + 1);
                    if (card[0] > member[choose].card[0]){
                        cout << tar << "死亡" << endl;
                        member[choose].isAlive = false;
                    }
                    else if (card[0] < member[choose].card[0]){
                        cout << name << "死亡" << endl;
                        isAlive = false;
                    }
                }
                break;
                
                
               

            }
            
            case 3:{
                cout << name << "打出3.侍女" << endl;
                cout << name << "无法被选中，持续一回合" << endl;
                bechosen = false;
                break;

            }
            
            case 4:{
                cout << name << "打出4.王子" << endl;
                int choose = cur_id - 1;
                do{
                    choose = (choose + 1) % 4;
                }while(!member[choose].isAlive || !member[choose].bechosen);

                if (choose == cur_id - 1){
                    cout << name << "选择了自己作为技能目标" << endl;
                    cout << name << "的回合结束" << endl;
                    return;
                }
                else{
                    string tar = choose == 0 ? "你" : "玩家" + (char)(cur_id + '0');
                    bool someone_dead = member[choose].throw_card(rest, count);
                    
                }
                break;
                
                
               

            }
            
            case 5:{
                cout << name << "打出5.国王" << endl;
                int choose = cur_id - 1;
                do{
                    choose = (choose + 1) % 4;
                }while(!member[choose].isAlive || !member[choose].bechosen);

                if (choose == cur_id - 1){
                    cout << name << "选择了自己作为技能目标" << endl;
                    cout << name << "的回合结束" << endl;
                    return;
                }
                else{
                    auto it = card[0];
                    card[0] = member[choose].card[0];
                    member[choose].card[0] = it;
                    
                }
                break;

            }
            
            case 6:{
                cout << name << "打出6.女伯爵" << endl;
                break;
            }
            
            
            
            case 7:{
                cout << name << "打出7.公主并死亡" << endl;
                isAlive = false;
                break;
            }
            default:{
                break;
            }
            
        }
        
    }

    //回合函数
    void turn(vector<int>& rest, vector<Player>& member, int& cur_id, int& count){
        if (!isAlive){
            return;
        }
        bechosen = true;
        int c = create_card(rest, count);
        count--;
        if (cur_id == 1){
            player_turn(rest, member ,c, count);
        }
        else{
            card.push_back(c);
            AI_turn(rest,member,cur_id, count);
        }

    }
    
        
};



void game(){
    //游戏的所有卡牌，用一个数组表示，表示还剩余多少张
    vector<int> rest(8,2);
    rest[0] = 5; //守卫为5张
    rest[5] = rest[6] = rest[7] = 1; //比较大的三张牌数量为1
    int count = 16;  //牌的数量
    create_card(rest, count); //游戏开始时默认少一张牌
    

    //创建玩家和AI对象
    Player player(create_card(rest, count), false, true);
    Player AI_1(create_card(rest, count), true, true);
    Player AI_2(create_card(rest, count), true, true);
    Player AI_3(create_card(rest, count), true, true);

    //玩家数组
    vector<Player> member = {player, AI_1, AI_2, AI_3};

    //当前玩家号码
    int cur_id = 1;

    while(member[0].isAlive + member[1].isAlive + member[2].isAlive + member[3].isAlive >= 1
    && count != 0){
        sleep(2);
        //先处理场上只有一名玩家存活的情况
        if (member[0].isAlive + member[1].isAlive + member[2].isAlive + member[3].isAlive == 1){
            if (member[0].isAlive){
                cout << "只有你存活！" << endl;
                cout << "你获得胜利！" << endl;
                return;
            }
            else if (member[1].isAlive){
                cout << "只有玩家2存活！" << endl;
                cout << "玩家2获得胜利！" << endl;
                return;
            }
            else if (member[2].isAlive){
                cout << "只有玩家3存活！" << endl;
                cout << "玩家3获得胜利！" << endl;
                return;
            }
            else{
                cout << "只有玩家4存活！" << endl;
                cout << "玩家4获得胜利！" << endl;
                return;
            }
            

        }

        //游戏继续
        member[cur_id - 1].turn(rest, member, cur_id,count);
        cur_id++;
        if (cur_id == 5){
            cur_id = 1;
        }

    }
    if (count == 0){
        vector<int> win_id;
        int max = -1;
        for (int i = 0; i < 4; i++){
            if (member[i].isAlive){
                if (member[i].card[0] == max){
                    win_id.push_back(i);
                }
                else if(member[i].card[0] > max){
                    win_id.clear();
                    win_id.push_back(i);
                    max = member[i].card[0];
                }

            }
            
        }

        for (int id : win_id){
            if (id == 0){
                cout << "你的点数为" << max << "." << card_name[max] << endl;
                cout << "你获得胜利" << endl;
            }
            else{
                cout << "玩家" << id + 1 << "的点数为" << max << "." << card_name[max] << endl;
                cout << "玩家" << id + 1 << "获得胜利" << endl;                
            }
            
        }

    }
    cout << "游戏结束" << endl;
}

int main(){
    srand((unsigned int)time(NULL));
    int input;
    start_menu();
    do{
        cout << "输入0退出，输入1开始游戏: ";
        if (!(cin >> input)){
            cin.clear();
            cin.ignore(10000, '\n');
            input = -1;
            continue;
        }
        switch (input){
            case 0:
                cout << "感谢您参与游戏" << endl;
                break;
            case 1:{
                cout << "游戏开始" << endl;
                game();
                break;
            }
            default:
                cout << "输入错误！" << endl;
                break;
        }
    } while (input);
    
}

