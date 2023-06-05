#include <DxLib.h>
#include <vector>
#include <random>
using namespace std;

// �E�B���h�E�T�C�Y
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// �p�Y���̃T�C�Y
const int PUZZLE_SIZE = 4;
const int PUZZLE_PIECE_SIZE = 80;

// �p�Y���s�[�X�̐F
const int PIECE_COLOR = GetColor(255, 255, 255);

// �p�Y���̏�Ԃ�\��2�����z��
std::vector<std::vector<int>> puzzleBoard(PUZZLE_SIZE, std::vector<int>(PUZZLE_SIZE));

// �󂫃X�y�[�X�̈ʒu
int emptyRow = PUZZLE_SIZE - 1;
int emptyCol = PUZZLE_SIZE - 1;

// �����_���Ȑ��l�𐶐����邽�߂̗����G���W��
std::random_device rd;
std::mt19937 rng(rd());

// �Q�[���̏�����
void InitializeGame()
{
    // �p�Y���̏����z�u�𐶐�
    int number = 1;
    for (int row = 0; row < PUZZLE_SIZE; ++row)
    {
        for (int col = 0; col < PUZZLE_SIZE; ++col)
        {
            puzzleBoard[row][col] = number;
            ++number;
        }
    }
    puzzleBoard[emptyRow][emptyCol] = 0;

    // �p�Y�����V���b�t������
    std::shuffle(puzzleBoard.begin(), puzzleBoard.end(), rng);
}

// �p�Y����`�悷��
void DrawPuzzle()
{
    for (int row = 0; row < PUZZLE_SIZE; ++row)
    {
        for (int col = 0; col < PUZZLE_SIZE; ++col)
        {
            int number = puzzleBoard[row][col];
            if (number != 0)
            {
                int x = col * PUZZLE_PIECE_SIZE;
                int y = row * PUZZLE_PIECE_SIZE;
                DrawBox(x, y, x + PUZZLE_PIECE_SIZE, y + PUZZLE_PIECE_SIZE, PIECE_COLOR, TRUE);
                DrawFormatString(x + 10, y + 10, GetColor(0, 0, 0), "%d", number);
            }
        }
    }
}

// �p�Y���s�[�X�̈ړ�
void MovePiece(int row, int col)
{
    if ((row == emptyRow && (col == emptyCol - 1 || col == emptyCol + 1)) ||
        (col == emptyCol && (row == emptyRow - 1 || row == emptyRow + 1)))
    {
        puzzleBoard[emptyRow][emptyCol] = puzzleBoard[row][col];
        puzzleBoard[row][col] = 0;
        emptyRow = row;
        emptyCol = col;
    }
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // �E�B���h�E���쐬
    ChangeWindowMode(TRUE);
    SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
    SetMainWindowText("15 Puzzle");
    if (DxLib_Init() == -1)
    {
        return -1;
    }

    // �Q�[���̏�����
    InitializeGame();

    // �^�C�}�[�̊J�n
    int startTime = GetNowCount();

    while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        if (ClearDrawScreen() == 0)
        {
            // �p�Y����`��
            DrawPuzzle();

            // �p�Y���s�[�X�̈ړ�
            int mouseX, mouseY;
            GetMousePoint(&mouseX, &mouseY);

            int row = mouseY / PUZZLE_PIECE_SIZE;
            int col = mouseX / PUZZLE_PIECE_SIZE;

            if (GetMouseInput() & MOUSE_INPUT_LEFT)
            {
                MovePiece(row, col);
            }

            // �p�Y�����N���A���ꂽ���`�F�b�N
            bool isClear = true;
            int number = 1;
            for (int row = 0; row < PUZZLE_SIZE; ++row)
            {
                for (int col = 0; col < PUZZLE_SIZE; ++col)
                {
                    if (puzzleBoard[row][col] != number)
                    {
                        isClear = false;
                        break;
                    }
                    ++number;
                }
                if (!isClear)
                {
                    break;
                }
            }

            // �N���A�����ꍇ�A�^�C�}�[���~���ăN���A�^�C����\��
            if (isClear)
            {
                int endTime = GetNowCount();
                int clearTime = endTime - startTime;
                DrawFormatString(10, WINDOW_HEIGHT - 30, GetColor(255, 255, 255), "Clear Time: %d ms", clearTime);
            }

            ScreenFlip();
        }
    }

    DxLib_End();
    return 0;
}
