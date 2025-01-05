#include <iostream>
#include <windows.h>

using namespace std;

int board[4][4] = {
    {0, 0, 1, 0},
    {0, 4, 0, 0},
    {0, 0, 2, 0},
    {0, 1, 0, 0},
};
int ans[4][4] = {
    {3, 2, 1, 4},
    {1, 4, 3, 2},
    {4, 3, 2, 1},
    {2, 1, 4, 3},
};
bool editable[4][4];
bool h,v,b;
int cur_r = 0, cur_c = 0;
void check_horizontal(int c,int cur_r) {
    h=false;
	for(int i=0;i<4;++i){
    if(board[cur_r][i] ==c ){
    h=true;    //如果橫排有一樣的數字，回傳true
}
else if(board[cur_r][i] !=c&& board[cur_r][i] !=0) h=false; //如果橫排沒有一樣的數字，而且完成橫排，回傳false
}
}

void check_vertical(int c,int cur_r)
{	v=false;
	for(int i=0;i<4;++i){
    if(board[i][cur_c] ==c ){
    v=true;    //如果直排有一樣的數字，回傳true
}
else if(board[i][cur_c] !=c&& board[i][cur_c] !=0) v=false; //如果直排沒有一樣的數字，而且完成直排，回傳false
}
}

void check_block(int c,int cur_r,int cur_c)
{	b=false;
	if(cur_r%2!=0) cur_r-=1;
	 if(cur_c%2!=0) cur_c-=1;
	 for(int i=0;i<2;++i){
 		for(int j=0;j<2;++j)
    if(board[cur_r+i][cur_c+j] ==c ){
    	b=true;    //如果大格有一樣的數字，回傳true
}
}
}

void fill_number(int c)
{
if(editable[cur_r][cur_c]){ //如果 [cur_r][cur_c]可編輯
board[cur_r][cur_c] =c;//把[cur_r][cur_c]變成c
    check_horizontal(c,cur_r);//檢查有沒有重複的數字
    check_vertical(c,cur_c);
    check_block(c,cur_r,cur_c);
}
}
string get_styled_text(string text, string style)
{
    string color = "", font = "";
    for (char c : style)
    {
        if (c == 'R')
            color = "31";
        if (c == 'G')
            color = "32";
        if (c == 'E')
            color = "41";
        if (c == 'C')
            color = "106";
        if (c == 'B')
            font = ";1";
    }
    return "\x1b[" + color + font + "m" + text + "\x1b[0m";
}
void move_cursor(char c)
{
    int target_r = cur_r; // 檢查後的新行座標
    int target_c = cur_c; // 檢查後的新列座標

    if (c == 'W' || c == 'w') { // 往上移動
        for (int i = cur_r - 1; i >= 0; --i) {
            if (editable[i][cur_c]) { // 檢查是否可編輯
                target_r = i; // 設定目標座標
                break;        // 找到可編輯格子後停止檢查
            }
        }
    }
    else if (c == 'S' || c == 's') { // 往下移動
        for (int i = cur_r + 1; i <= 3; ++i) {
            if (editable[i][cur_c]) { // 檢查是否可編輯
                target_r = i; // 設定目標座標
                break;        // 找到可編輯格子後停止檢查
            }
        }
    }
    else if (c == 'A' || c == 'a') { // 往左移動
        for (int j = cur_c - 1; j >= 0; --j) {
            if (editable[cur_r][j]) { // 檢查是否可編輯
                target_c = j; // 設定目標座標
                break;        // 找到可編輯格子後停止檢查
            }
        }
    }
    else if (c == 'D' || c == 'd') { // 往右移動
        for (int j = cur_c + 1; j <= 3; ++j) {
            if (editable[cur_r][j]) { // 檢查是否可編輯
                target_c = j; // 設定目標座標
                break;        // 找到可編輯格子後停止檢查
            }
        }
    }

    // 更新光標位置
    cur_r = target_r;
    cur_c = target_c;
}

bool is_invalid(int i, int j) {
    int c = board[i][j]; // 當前位置的數字
    if (c == 0) return false; // 如果該位置是空的，不存在衝突
	bool conflict[4][4]={false};
    bool row_conflict = false, col_conflict = false, block_conflict = false;

    // 檢查橫排是否有衝突
    for (int col = 0; col < 4; ++col) {
        if (col != j && board[i][col] == c) {
            row_conflict = true;
            break;
        }
    }

    // 檢查直排是否有衝突
    for (int row = 0; row < 4; ++row) {
        if (row != i && board[row][j] == c) {
            col_conflict = true;
            break;
        }
    }

    // 檢查大格是否有衝突
    int block_row_start = (i / 2) * 2;
    int block_col_start = (j / 2) * 2;
    for (int row = block_row_start; row < block_row_start + 2; ++row) {
        for (int col = block_col_start; col < block_col_start + 2; ++col) {
            if ((row != i || col != j) && board[row][col] == c) {
                block_conflict = true;
                break;
            }
        }
    }

    // 如果任意條件有衝突，將相關區域標記
    if (row_conflict || col_conflict || block_conflict) {
        // 如果有橫排衝突
        if (row_conflict) {
            for (int col = 0; col < 4; ++col) {
                conflict[i][col] = true; // 假設 conflict 是一個 2D 陣列，記錄衝突
            }
        }
        // 如果有直排衝突
        if (col_conflict) {
            for (int row = 0; row < 4; ++row) {
                conflict[row][j] = true;
            }
        }
        // 如果有大格衝突
        if (block_conflict) {
            for (int row = block_row_start; row < block_row_start + 2; ++row) {
                for (int col = block_col_start; col < block_col_start + 2; ++col) {
                    conflict[row][col] = true;
                }
            }
        }
        return true; // 有衝突
    }

    return false; // 無衝突
}


bool is_done(int i, int j)
{	int block_row_start = (i / 2) * 2;
    int block_col_start = (j / 2) * 2;
	int done[4][4]={false};	
	if(h=false){
		for (int col = 0; col < 4; ++col) {
                done[i][col] = true; 
        }
}
	if(v=false){
	for (int row = 0; row < 4; ++row) {
                done[row][j] = true; 
            }
}
	if (b=false) {
            for (int row = block_row_start; row < block_row_start + 2; ++row) {
                for (int col = block_col_start; col < block_col_start + 2; ++col) {
                    done[row][col] = true;
                }
            }
   
} return false;
}

bool check_win() {
    for (int i = 0; i < 4; ++i) {
        int t_h = 0; // 檢查所有橫排是否符合條件
        for (int j = 0; j < 4; ++j) {
            t_h += board[i][j];
        }
        if (t_h != 10) {
            return false; // 若有任何一橫排不符合條件，直接返回 false
        }
    }
    for (int j = 0; j < 4; ++j) {// 檢查所有直排是否符合條件
        int t_v = 0;
        for (int i = 0; i < 4; ++i) {
            t_v += board[i][j];
        }
        if (t_v != 10) {
            return false; // 若有任何一直排不符合條件，直接返回 false
        }
    }
   for (int i = 0; i < 4; i += 2) { // 檢查所有 2x2 大格是否符合條件
        for (int j = 0; j < 4; j += 2) {
            int t_b = 0;
            for (int m = 0; m < 2; ++m) {
                for (int n = 0; n < 2; ++n) {
                    t_b += board[i + m][j + n];
                }
            }
            if (t_b != 10) {
                return false; // 若有任何一個大格不符合條件，直接返回 false
            }
        }
    }

    // 若所有條件都符合，返回 true
    return true;
}



bool is_moving_action(char c)
{
    return (c == 'W' || c == 'w' || c == 'S' || c == 's' ||
            c == 'A' || c == 'a' || c == 'D' || c == 'd');
}

bool is_filling_action(char c)
{   return (c >= '0' && c <= '4');
}
void print_board()
{
    // Flush the screen
    cout << "\x1b[2J\x1b[1;1H";

    // Print usage hint.
    cout << get_styled_text("W/A/S/D: ", "B") << "move cursor" << endl;
    cout << get_styled_text("    1-4: ", "B") << "fill in number" << endl;
    cout << get_styled_text("      0: ", "B") << "clear the cell" << endl;
    cout << get_styled_text("      Q: ", "B") << "quit" << endl;
    cout << get_styled_text("      Z: ", "B") << "quit and show me answer" << endl;
    cout << endl;

    // Iterate through and print each cell.
    for (int i = 0; i < 4; ++i)
    {
        // Print horizontal divider.
        if (i && i % 2 == 0)
            cout << "---------------" << endl;

        // Print the first vertical divider in each line.
        cout << "|";
        for (int j = 0; j < 4; ++j)
        {
            // Set text style based on the state of the cell.
            string style = "";
            // Set style for the cell the cursor pointing to.
            if (cur_r == i && cur_c == j)
                style = "C";
            // Set style for the cell in an invalid line.
            else if (is_invalid(i, j))
                style = "E";
            // Set style for the cell in a finished line.
            else if (is_done(i, j))
                style = "G";

            // Set style for a the cell that is immutable.
            if (!editable[i][j])
                style += "B";

            // Print the cell out in styled text.
            // If the content is 0, print a dot, else print the number.
            if (board[i][j] == 0)
                cout << get_styled_text(" · ", style);
            else
                cout << get_styled_text(" " + to_string(board[i][j]) + " ", style);

            // Print the vertical divider for each block.
            if ((j + 1) % 2 == 0)
                cout << "|";
        }
        cout << endl;
    }
}

void initialize()
{
    // Set up styled text for Windows.
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // Mark editable cells
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            editable[i][j] = !board[i][j];

    // Print the initial board out.
    print_board();
}

int main()
{
    char c;
    bool action_ok;

    initialize();
    while (cin >> c)
    {
        action_ok = false;
        if (is_moving_action(c))
        {
            action_ok = true;
            move_cursor(c);
        }

        if (is_filling_action(c))
        {
            action_ok = true;
            fill_number(c-48);//把輸入的字元變成數字，把 ASCII CODE變成 0~4
        }

        if (c == 'Q' || c == 'q')
            break;
		if (c == 'Z' || c == 'z'){
			action_ok = true;
			cout<<"answer:"<<endl;
			for(int i=0;i<4;++i){
				for(int j=0;j<4;++j){
					cout<<ans[i][j]<<" ";
				}
				cout<<endl;
			}
			break;
		}
        print_board();

        if (check_win())
        {
            cout << "YOU WIN!" << endl;
            break;
        }

        if (!action_ok)
            cout << get_styled_text("!!! Invalid action !!!", "R");
    }

    return 0;
}
 
