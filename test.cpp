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
void fill_number(int c)
{
if(editable[cur_r][cur_c]){ //�p�G [cur_r][cur_c]�i�s��
board[cur_r][cur_c] =c;//��[cur_r][cur_c]�ܦ�c
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
    int target_r = cur_r; // �ˬd�᪺�s��y��
    int target_c = cur_c; // �ˬd�᪺�s�C�y��

    if (c == 'W' || c == 'w') { // ���W����
        for (int i = cur_r - 1; i >= 0; --i) {
            if (editable[i][cur_c]) { // �ˬd�O�_�i�s��
                target_r = i; // �]�w�ؼЮy��
                break;        // ���i�s���l�ᰱ���ˬd
            }
        }
    }
    else if (c == 'S' || c == 's') { // ���U����
        for (int i = cur_r + 1; i <= 3; ++i) {
            if (editable[i][cur_c]) { // �ˬd�O�_�i�s��
                target_r = i; // �]�w�ؼЮy��
                break;        // ���i�s���l�ᰱ���ˬd
            }
        }
    }
    else if (c == 'A' || c == 'a') { // ��������
        for (int j = cur_c - 1; j >= 0; --j) {
            if (editable[cur_r][j]) { // �ˬd�O�_�i�s��
                target_c = j; // �]�w�ؼЮy��
                break;        // ���i�s���l�ᰱ���ˬd
            }
        }
    }
    else if (c == 'D' || c == 'd') { // ���k����
        for (int j = cur_c + 1; j <= 3; ++j) {
            if (editable[cur_r][j]) { // �ˬd�O�_�i�s��
                target_c = j; // �]�w�ؼЮy��
                break;        // ���i�s���l�ᰱ���ˬd
            }
        }
    }

    // ��s���Ц�m
    cur_r = target_r;
    cur_c = target_c;
}

bool is_invalid(int i, int j) {
    int c = board[i][j]; // ��e��m���Ʀr
    if (c == 0) return false; // �p�G�Ӧ�m�O�Ū��A���s�b�Ĭ�
	bool conflict[4][4]={false};
    bool row_conflict = false, col_conflict = false, block_conflict = false;

    // �ˬd��ƬO�_���Ĭ�
    for (int col = 0; col < 4; ++col) {
        if (col != j && board[i][col] == c) {
            row_conflict = true;
            break;
        }
    }

    // �ˬd���ƬO�_���Ĭ�
    for (int row = 0; row < 4; ++row) {
        if (row != i && board[row][j] == c) {
            col_conflict = true;
            break;
        }
    }

    // �ˬd�j��O�_���Ĭ�
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

    // �p�G���N���󦳽Ĭ�A�N�����ϰ�аO
    if (row_conflict || col_conflict || block_conflict) {
        // �p�G����ƽĬ�
        if (row_conflict) {
            for (int col = 0; col < 4; ++col) {
                conflict[i][col] = true; // ���] conflict �O�@�� 2D �}�C�A�O���Ĭ�
            }
        }
        // �p�G�����ƽĬ�
        if (col_conflict) {
            for (int row = 0; row < 4; ++row) {
                conflict[row][j] = true;
            }
        }
        // �p�G���j��Ĭ�
        if (block_conflict) {
            for (int row = block_row_start; row < block_row_start + 2; ++row) {
                for (int col = block_col_start; col < block_col_start + 2; ++col) {
                    conflict[row][col] = true;
                }
            }
        }
        return true; // ���Ĭ�
    }

    return false; // �L�Ĭ�
}


bool is_done(int i, int j) {
    int t_h = 0, t_v = 0, t_b = 0;  // �p���B�C�B�j�檺�[�`
    bool h = false, v = false, b = false;  // �Ψ��ˬd��B�C�B�j�椤�O�_�]�t0

    // �ˬd�檺�M
    for (int l = 0; l < 4; ++l) {
        t_h += board[i][l];
        if (board[i][l] == 0) h = true;  // �p�G�椤��0�A�аO��true
    }
    if (t_h == 10 && !h) {  // �p�G�檺�[�`����10�B�椤�S��0�A�^��true
        return true;
    }

    // �ˬd�C���M
    for (int l = 0; l < 4; ++l) {
        t_v += board[l][j];
        if (board[l][j] == 0) v = true;  // �p�G�C����0�A�аO��true
    }
    if (t_v == 10 && !v) {  // �p�G�C���[�`����10�B�C���S��0�A�^��true
        return true;
    }

    // �p��Ҧb�j�檺�_�l��m
    int cur_r = (i / 2) * 2;  // �T�w�j�檺�_�l��
    int cur_c = (j / 2) * 2;  // �T�w�j�檺�_�l�C

    // �ˬd�j�檺�M (2x2 �ϰ�)
    for (int k = 0; k < 2; ++k) {  // �M��2x2�j�椤���C�@��
        for (int l = 0; l < 2; ++l) {  // �M��2x2�j�椤���C�@�C
            t_b += board[cur_r + k][cur_c + l];
            if (board[cur_r + k][cur_c + l] == 0) b = true;  // �p�G�j�椤��0�A�аO��true
        }
    }
    if (t_b == 10 && !b) {  // �p�G�j�檺�[�`����10�B�j�椤�S��0�A�^��true
        return true;
    }

    // �p�G���S����������A��^ false
    return false;
}




bool check_win() {
    for (int i = 0; i < 4; ++i) {
        int t_h = 0; // �ˬd�Ҧ���ƬO�_�ŦX����
        for (int j = 0; j < 4; ++j) {
            t_h += board[i][j];
        }
        if (t_h != 10) {
            return false; // �Y������@��Ƥ��ŦX����A������^ false
        }
    }
    for (int j = 0; j < 4; ++j) {// �ˬd�Ҧ����ƬO�_�ŦX����
        int t_v = 0;
        for (int i = 0; i < 4; ++i) {
            t_v += board[i][j];
        }
        if (t_v != 10) {
            return false; // �Y������@���Ƥ��ŦX����A������^ false
        }
    }
   for (int i = 0; i < 4; i += 2) { // �ˬd�Ҧ� 2x2 �j��O�_�ŦX����
        for (int j = 0; j < 4; j += 2) {
            int t_b = 0;
            for (int m = 0; m < 2; ++m) {
                for (int n = 0; n < 2; ++n) {
                    t_b += board[i + m][j + n];
                }
            }
            if (t_b != 10) {
                return false; // �Y������@�Ӥj�椣�ŦX����A������^ false
            }
        }
    }

    // �Y�Ҧ����󳣲ŦX�A��^ true
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
                cout << get_styled_text(" �P ", style);
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
            fill_number(c-48);//���J���r���ܦ��Ʀr�A�� ASCII CODE�ܦ� 0~4
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

