#include<iostream>
#include<cstring>
using namespace std;

void PrintBoard(char arr[7][6]){
    for(int i = 5;i>=0;i--){
        for(int j = 0;j<7;j++){
            if(arr[j][i] == '0'){
                cout<<". ";
            }
            if(arr[j][i] == 'B'){
                cout<<"B ";
            }
            if(arr[j][i] == 'R'){
                cout<<"R ";
            }
        }
        cout<<endl;
    }
}

int EnterMove(char color,int colnum,char board[7][6]){
    colnum--;
    int i = 0;
    for(;i<6 && colnum >=0 && colnum<7;i++){
        if(board[colnum][i] == '0'){
            board[colnum][i] = color;
            break;
        }
    }
    if(colnum < 0 || colnum >= 7 || i == 6){
        return 1;
    }
    return 0;
}


int StaticEvalutaion(char board[7][6],char colord,bool* isTerminal){
    int values[7] = {0};
    for(int x = 0;x<7;x++){
        for(int y = 5;y>=0;y--){
            if(board[x][y] != '0'){
                char color = board[x][y];
                int diag[4] = {0};            //tl tr br bl
                int line[3] = {0};            //left bottom right

                for(int i = x+1; i>=0 && i<7;i++){
                    if(board[i][y] == color) line[2] += 1;
                    else break;
                }
                for(int i = x-1; i>=0 && i<7;i--){
                    if(board[i][y] == color) line[0] += 1;
                    else break;
                }
                for(int j = y-1;j>=0 && j<6;j--){
                    if(board[x][j] == color) line[1] += 1;
                    else break;
                }

                for(int i = x-1,j=y+1; i>=0 && i<7 &&  j>=0 && j<6 ;i--,j++){
                    if(board[i][j] == color) diag[0] += 1;
                    else break;
                }
                for(int i = x+1,j=y+1; i>=0 && i<7 &&  j>=0 && j<6 ;i++,j++){
                    if(board[i][j] == color) diag[1] += 1;
                    else break;
                }
                for(int i = x + 1,j=y-1; i>=0 && i<7 &&  j>=0 && j<6 ;i++,j--){
                    if(board[i][j] == color) diag[2] += 1;
                    else break;
                }
                for(int i = x-1,j=y-1; i>=0 && i<7 &&  j>=0 && j<6 ;i--,j--){
                    if(board[i][j] == color) diag[3] += 1;
                    else break;
                }

                int state[4] = {0};  // number of lines from current piece
                state[diag[2]+diag[0]] += 1;
                state[diag[1]+diag[3]] += 1;
                state[line[0]+line[2]] += 1;
                state[line[1]] += 1;
                values[x] = state[3]*20 + state[2]*5 + state[1];
                if(board[x][y] != colord){values[x] *= -1;}
                if(state[3] > 0) *isTerminal = true; 
                break;
            }
        }
    }
    int sum = 0;
    for(int k = 0;k<7;k++) sum += values[k];
    return sum;
}

int minimax(char board[7][6],int depth,bool maximizingPlayer){       // assuming bot is black

    bool isTerminal = false;
    int eval = StaticEvalutaion(board,'B',&isTerminal);
    if(depth == 0 || isTerminal){
        return eval;
    }

    //legal moves
    int moves[7] = {-1,-1,-1,-1,-1,-1,-1};
    for(int i = 0;i<7;i++){
        for(int j = 0;j<6;j++){
            if(board[i][j] == '0'){
                moves[i] = j;
                break;
            }
        }
    }

    int value;
    if(maximizingPlayer){
        value = INT_MIN;
        for(int i  = 0;i<7;i++){
            if(moves[i] == -1) continue;
            EnterMove('B',i+1,board);
            value = max(value,minimax(board,depth-1,false));
            board[i][moves[i]] = '0'; 
        }
    }
    else{
        value = INT_MAX;
        for(int i  = 0;i<7;i++){
            if(moves[i] == -1) continue;
            EnterMove('R',i+1,board);
            value = min(value,minimax(board,depth-1,true));
            board[i][moves[i]] = '0'; 
        }
    }

    return value;
}

int main(){
    char board[7][6] = {0};
    memset(board,'0',42);
    int blackTurn = 0;
    while (true){
        bool ended = false;
        if(!blackTurn) PrintBoard(board);
        if(blackTurn){
            int moves[7] = {-1,-1,-1,-1,-1,-1,-1};
            for(int i = 0;i<7;i++){
                for(int j = 0;j<6;j++){
                    if(board[i][j] == '0'){
                        moves[i] = j;
                        break;
                    }
                }
            }
            int values[7] = {0};
            for(int i = 0;i<7;i++){
                if(moves[i] != -1){
                    EnterMove('B',i+1,board);
                    values[i] = minimax(board,6,false);
                    board[i][moves[i]] = '0'; 
                }
            }

            int maxi = 0;
            int max = INT_MIN;
            for(int i = 0;i<7;i++){
                if(moves[i] != -1 && values[i] >= max){
                    max = values[i];
                    maxi = i;
                }

            }

            EnterMove('B',maxi+1,board);
            blackTurn = 0;
            StaticEvalutaion(board,'B',&ended);
        }
        else{
            int colnum;
            cin>>colnum;
            if(!EnterMove('R',colnum,board)) blackTurn = 1;
            else cout<<"bad move \n";
            StaticEvalutaion(board,'B',&ended);
        }
        if(ended == true){
            PrintBoard(board);
            cout<<"GAME ENDED "<<endl;
            break;
        }
    }
}