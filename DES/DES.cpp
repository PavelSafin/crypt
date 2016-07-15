#include "DES.h"


DES::DES() { }

DES::DES(string &key) {
    set_key(key);
}

DES::~DES() { }

void DES::set_message(string &message) {
    this->message = message;
}

string DES::get_message() {
    return this->message;
}

void DES::set_key(string &key) {
    this->key = key;
    generate_keys();
}

void DES::generate_keys() {
    this->keys.resize(17);
    int c0[28] = {57, 49, 41, 33, 25, 17, 9,
                  1, 58, 50, 42, 34, 26, 18,
                  10, 2, 59, 51, 43, 35, 27,
                  19, 11, 3, 60, 52, 44, 36};

    int d0[28] = {63, 55, 47, 39, 31, 23, 15,
                  7, 62, 54, 46, 38, 30, 22,
                  14, 6, 61, 53, 45, 37, 29,
                  21, 13, 5, 28, 20, 12, 4};

    for (int i = 0; i < 64; ++i)
        this->keys[0].push_back(rand() % 2);

    int size = 0;
    vector<int> m(this->keys[0]);

    for (int i = 0; i < 32; ++i)
        if ((i + 1) % 8 != 0) {
            this->keys[0][size] = m[c0[size] - 1];
            this->keys[0][size + 28] = m[d0[size] - 1];
            ++size;
        }

    for (int i = 1; i < 17; ++i)
        this->keys[i] = next_key(this->keys[i - 1], i);

    this->keys[16] = this->keys[0];
}

void DES::split(vector<int> &left, vector<int> &right, vector<int> &target, int size) {
    for (int i = 0; i < size; ++i) {
        left[i] = target[i + size];
        right[i] = target[i];
    }

}

void DES::merge(vector<int> &left, vector<int> &right, vector<int> &target, int size) {
    for (int i = 0; i < size; ++i) {
        target[i + size] = left[i];
        target[i] = right[i];
    }
}

int DES::bin_vector_to_char(vector<int> &m) {
    int c = 0;
    for (int i = 0; i < 8; ++i) {
        c += m[i] << (7 - i);
    }
    return c;
}

void DES::char_to_bin_vector(char c, vector<int> &m, int l) {
    for (int i = 7; i >= 0; --i) {
        m[l] = c & 1;
        c >>= 1;
        --l;
    }
}

void DES::key_permutation(vector<int> &key) {
    int k[48] = {14, 17, 11, 24, 1, 5, 3, 28,
                 15, 6, 21, 10, 23, 19, 12, 4,
                 26, 8, 16, 7, 27, 20, 13, 2,
                 41, 52, 31, 37, 47, 55, 30, 40,
                 51, 45, 33, 48, 44, 49, 39, 56,
                 34, 53, 46, 42, 50, 36, 29, 32};
    int kp[48];
    for (int i = 0; i < 48; ++i)
        kp[i] = key[k[i] - 1];

    for (int i = 0; i < 48; ++i)
        key[i] = kp[i];
}

vector<int> DES::next_key(vector<int> &key, int n) {
    int shift[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

    vector<int> c(28);
    vector<int> d(28);
    vector<int> target(key);

    this->split(d, c, target, 28);

    rotate(c.begin(), c.begin() + shift[n - 1], c.end());
    rotate(d.begin(), d.begin() + shift[n - 1], d.end());

    this->merge(d, c, target, 28);
    this->key_permutation(key);

    return target;
}

void DES::initial_permutation(vector<int> &m) {
    int p[64] = {58, 50, 42, 34, 26, 18, 10, 2,
                 60, 52, 44, 36, 28, 20, 12, 4,
                 62, 54, 46, 38, 30, 22, 14, 6,
                 64, 56, 48, 40, 32, 24, 16, 8,
                 57, 49, 41, 33, 25, 17, 9, 1,
                 59, 51, 43, 35, 27, 19, 11, 3,
                 61, 53, 45, 37, 29, 21, 13, 5,
                 63, 55, 47, 39, 31, 23, 15, 7};

    int ip[64];
    for (int i = 0; i < 64; ++i)
        ip[i] = m[p[i] - 1];

    for (int i = 0; i < 64; ++i)
        m[i] = ip[i];
}

void DES::final_permutation(vector<int> &m) {
    int p[64] = {40, 8, 48, 16, 56, 24, 64, 32,
                 39, 7, 47, 15, 55, 23, 63, 31,
                 38, 6, 46, 14, 54, 22, 62, 30,
                 37, 5, 45, 13, 53, 21, 61, 29,
                 36, 4, 44, 12, 52, 20, 60, 28,
                 35, 3, 43, 11, 51, 19, 59, 27,
                 34, 2, 42, 10, 50, 18, 58, 26,
                 33, 1, 41, 9, 49, 17, 57, 25};

    vector<int> fp(64);
    for (int i = 0; i < 64; ++i)
        fp[i] = m[p[i] - 1];

    m = fp;
}

void DES::expand(vector<int> &m) {
    int e[48] = {32, 1, 2, 3, 4, 5, 4, 5,
                 6, 7, 8, 9, 8, 9, 10, 11,
                 12, 13, 12, 13, 14, 15, 16, 17,
                 16, 17, 18, 19, 20, 21, 20, 21,
                 22, 23, 24, 25, 24, 25, 26, 27,
                 28, 29, 28, 29, 30, 31, 32, 1};

    vector<int> em(48);
    for (int i = 0; i < 48; ++i)
        em[i] = m[e[i] - 1];

    m = em;
}

void DES::foo(vector<int> &subblock, vector<int> &key) {
    expand(subblock);
    for (int i = 0; i < 48; ++i)
        subblock[i] ^= key[i];


    int S[8][4][16];

    S[0][0][0] = 14;
    S[0][0][1] = 4;
    S[0][0][2] = 13;
    S[0][0][3] = 1;
    S[0][0][4] = 2;
    S[0][0][5] = 15;
    S[0][0][6] = 11;
    S[0][0][7] = 8;
    S[0][0][8] = 3;
    S[0][0][9] = 10;
    S[0][0][10] = 6;
    S[0][0][11] = 12;
    S[0][0][12] = 5;
    S[0][0][13] = 9;
    S[0][0][14] = 0;
    S[0][0][15] = 7;

    S[0][1][0] = 0;
    S[0][1][1] = 15;
    S[0][1][2] = 7;
    S[0][1][3] = 4;
    S[0][1][4] = 14;
    S[0][1][5] = 2;
    S[0][1][6] = 13;
    S[0][1][7] = 1;
    S[0][1][8] = 10;
    S[0][1][9] = 6;
    S[0][1][10] = 12;
    S[0][1][11] = 11;
    S[0][1][12] = 9;
    S[0][1][13] = 5;
    S[0][1][14] = 3;
    S[0][1][15] = 8;

    S[0][2][0] = 4;
    S[0][2][1] = 1;
    S[0][2][2] = 14;
    S[0][2][3] = 8;
    S[0][2][4] = 13;
    S[0][2][5] = 6;
    S[0][2][6] = 2;
    S[0][2][7] = 11;
    S[0][2][8] = 15;
    S[0][2][9] = 12;
    S[0][2][10] = 9;
    S[0][2][11] = 7;
    S[0][2][12] = 3;
    S[0][2][13] = 10;
    S[0][2][14] = 5;
    S[0][2][15] = 0;

    S[0][3][0] = 15;
    S[0][3][1] = 12;
    S[0][3][2] = 8;
    S[0][3][3] = 2;
    S[0][3][4] = 4;
    S[0][3][5] = 9;
    S[0][3][6] = 1;
    S[0][3][7] = 7;
    S[0][3][8] = 5;
    S[0][3][9] = 11;
    S[0][3][10] = 3;
    S[0][3][11] = 14;
    S[0][3][12] = 10;
    S[0][3][13] = 0;
    S[0][3][14] = 6;
    S[0][3][15] = 13;

    S[1][0][0] = 15;
    S[1][0][1] = 1;
    S[1][0][2] = 8;
    S[1][0][3] = 14;
    S[1][0][4] = 6;
    S[1][0][5] = 11;
    S[1][0][6] = 3;
    S[1][0][7] = 4;
    S[1][0][8] = 9;
    S[1][0][9] = 7;
    S[1][0][10] = 2;
    S[1][0][11] = 13;
    S[1][0][12] = 12;
    S[1][0][13] = 0;
    S[1][0][14] = 5;
    S[1][0][15] = 10;

    S[1][1][0] = 3;
    S[1][1][1] = 13;
    S[1][1][2] = 4;
    S[1][1][3] = 7;
    S[1][1][4] = 15;
    S[1][1][5] = 2;
    S[1][1][6] = 8;
    S[1][1][7] = 14;
    S[1][1][8] = 12;
    S[1][1][9] = 0;
    S[1][1][10] = 1;
    S[1][1][11] = 10;
    S[1][1][12] = 6;
    S[1][1][13] = 9;
    S[1][1][14] = 11;
    S[1][1][15] = 5;

    S[1][2][0] = 0;
    S[1][2][1] = 14;
    S[1][2][2] = 7;
    S[1][2][3] = 11;
    S[1][2][4] = 10;
    S[1][2][5] = 4;
    S[1][2][6] = 13;
    S[1][2][7] = 1;
    S[1][2][8] = 5;
    S[1][2][9] = 8;
    S[1][2][10] = 12;
    S[1][2][11] = 6;
    S[1][2][12] = 9;
    S[1][2][13] = 3;
    S[1][2][14] = 2;
    S[1][2][15] = 15;

    S[1][3][0] = 13;
    S[1][3][1] = 8;
    S[1][3][2] = 10;
    S[1][3][3] = 1;
    S[1][3][4] = 3;
    S[1][3][5] = 15;
    S[1][3][6] = 4;
    S[1][3][7] = 2;
    S[1][3][8] = 11;
    S[1][3][9] = 6;
    S[1][3][10] = 7;
    S[1][3][11] = 12;
    S[1][3][12] = 0;
    S[1][3][13] = 5;
    S[1][3][14] = 14;
    S[1][3][15] = 9;

    S[2][0][0] = 10;
    S[2][0][1] = 0;
    S[2][0][2] = 9;
    S[2][0][3] = 14;
    S[2][0][4] = 6;
    S[2][0][5] = 3;
    S[2][0][6] = 15;
    S[2][0][7] = 5;
    S[2][0][8] = 1;
    S[2][0][9] = 13;
    S[2][0][10] = 12;
    S[2][0][11] = 7;
    S[2][0][12] = 11;
    S[2][0][13] = 4;
    S[2][0][14] = 2;
    S[2][0][15] = 8;

    S[2][1][0] = 13;
    S[2][1][1] = 7;
    S[2][1][2] = 0;
    S[2][1][3] = 9;
    S[2][1][4] = 3;
    S[2][1][5] = 4;
    S[2][1][6] = 6;
    S[2][1][7] = 10;
    S[2][1][8] = 2;
    S[2][1][9] = 8;
    S[2][1][10] = 5;
    S[2][1][11] = 14;
    S[2][1][12] = 12;
    S[2][1][13] = 10;
    S[2][1][14] = 15;
    S[2][1][15] = 1;

    S[2][2][0] = 13;
    S[2][2][1] = 6;
    S[2][2][2] = 4;
    S[2][2][3] = 9;
    S[2][2][4] = 8;
    S[2][2][5] = 15;
    S[2][2][6] = 3;
    S[2][2][7] = 0;
    S[2][2][8] = 11;
    S[2][2][9] = 1;
    S[2][2][10] = 2;
    S[2][2][11] = 12;
    S[2][2][12] = 5;
    S[2][2][13] = 10;
    S[2][2][14] = 14;
    S[2][2][15] = 7;

    S[2][3][0] = 1;
    S[2][3][1] = 10;
    S[2][3][2] = 13;
    S[2][3][3] = 0;
    S[2][3][4] = 6;
    S[2][3][5] = 9;
    S[2][3][6] = 8;
    S[2][3][7] = 7;
    S[2][3][8] = 4;
    S[2][3][9] = 15;
    S[2][3][10] = 14;
    S[2][3][11] = 3;
    S[2][3][12] = 11;
    S[2][3][13] = 5;
    S[2][3][14] = 2;
    S[2][3][15] = 12;

    S[3][0][0] = 7;
    S[3][0][1] = 13;
    S[3][0][2] = 14;
    S[3][0][3] = 3;
    S[3][0][4] = 0;
    S[3][0][5] = 6;
    S[3][0][6] = 9;
    S[3][0][7] = 10;
    S[3][0][8] = 1;
    S[3][0][9] = 2;
    S[3][0][10] = 8;
    S[3][0][11] = 5;
    S[3][0][12] = 11;
    S[3][0][13] = 12;
    S[3][0][14] = 4;
    S[3][0][15] = 15;

    S[3][1][0] = 13;
    S[3][1][1] = 8;
    S[3][1][2] = 11;
    S[3][1][3] = 5;
    S[3][1][4] = 6;
    S[3][1][5] = 15;
    S[3][1][6] = 0;
    S[3][1][7] = 3;
    S[3][1][8] = 4;
    S[3][1][9] = 7;
    S[3][1][10] = 2;
    S[3][1][11] = 12;
    S[3][1][12] = 1;
    S[3][1][13] = 10;
    S[3][1][14] = 14;
    S[3][1][15] = 9;

    S[3][2][0] = 10;
    S[3][2][1] = 6;
    S[3][2][2] = 9;
    S[3][2][3] = 0;
    S[3][2][4] = 12;
    S[3][2][5] = 11;
    S[3][2][6] = 7;
    S[3][2][7] = 13;
    S[3][2][8] = 15;
    S[3][2][9] = 1;
    S[3][2][10] = 3;
    S[3][2][11] = 14;
    S[3][2][12] = 5;
    S[3][2][13] = 2;
    S[3][2][14] = 8;
    S[3][2][15] = 4;

    S[3][3][0] = 3;
    S[3][3][1] = 15;
    S[3][3][2] = 0;
    S[3][3][3] = 6;
    S[3][3][4] = 10;
    S[3][3][5] = 1;
    S[3][3][6] = 13;
    S[3][3][7] = 8;
    S[3][3][8] = 9;
    S[3][3][9] = 4;
    S[3][3][10] = 5;
    S[3][3][11] = 11;
    S[3][3][12] = 12;
    S[3][3][13] = 7;
    S[3][3][14] = 2;
    S[3][3][15] = 14;

    S[4][0][0] = 2;
    S[4][0][1] = 12;
    S[4][0][2] = 4;
    S[4][0][3] = 1;
    S[4][0][4] = 7;
    S[4][0][5] = 10;
    S[4][0][6] = 11;
    S[4][0][7] = 6;
    S[4][0][8] = 8;
    S[4][0][9] = 5;
    S[4][0][10] = 3;
    S[4][0][11] = 15;
    S[4][0][12] = 13;
    S[4][0][13] = 0;
    S[4][0][14] = 14;
    S[4][0][15] = 9;

    S[4][1][0] = 14;
    S[4][1][1] = 11;
    S[4][1][2] = 2;
    S[4][1][3] = 12;
    S[4][1][4] = 4;
    S[4][1][5] = 7;
    S[4][1][6] = 13;
    S[4][1][7] = 1;
    S[4][1][8] = 5;
    S[4][1][9] = 0;
    S[4][1][10] = 15;
    S[4][1][11] = 10;
    S[4][1][12] = 3;
    S[4][1][13] = 9;
    S[4][1][14] = 8;
    S[4][1][15] = 6;

    S[4][2][0] = 4;
    S[4][2][1] = 2;
    S[4][2][2] = 1;
    S[4][2][3] = 11;
    S[4][2][4] = 10;
    S[4][2][5] = 13;
    S[4][2][6] = 7;
    S[4][2][7] = 8;
    S[4][2][8] = 15;
    S[4][2][9] = 9;
    S[4][2][10] = 12;
    S[4][2][11] = 5;
    S[4][2][12] = 6;
    S[4][2][13] = 3;
    S[4][2][14] = 0;
    S[4][2][15] = 14;

    S[4][3][0] = 11;
    S[4][3][1] = 8;
    S[4][3][2] = 12;
    S[4][3][3] = 7;
    S[4][3][4] = 1;
    S[4][3][5] = 14;
    S[4][3][6] = 2;
    S[4][3][7] = 13;
    S[4][3][8] = 6;
    S[4][3][9] = 15;
    S[4][3][10] = 0;
    S[4][3][11] = 9;
    S[4][3][12] = 10;
    S[4][3][13] = 4;
    S[4][3][14] = 5;
    S[4][3][15] = 3;

    S[5][0][0] = 12;
    S[5][0][1] = 1;
    S[5][0][2] = 10;
    S[5][0][3] = 15;
    S[5][0][4] = 9;
    S[5][0][5] = 2;
    S[5][0][6] = 6;
    S[5][0][7] = 8;
    S[5][0][8] = 0;
    S[5][0][9] = 13;
    S[5][0][10] = 3;
    S[5][0][11] = 4;
    S[5][0][12] = 14;
    S[5][0][13] = 7;
    S[5][0][14] = 5;
    S[5][0][15] = 11;

    S[5][1][0] = 10;
    S[5][1][1] = 15;
    S[5][1][2] = 4;
    S[5][1][3] = 2;
    S[5][1][4] = 7;
    S[5][1][5] = 12;
    S[5][1][6] = 9;
    S[5][1][7] = 5;
    S[5][1][8] = 6;
    S[5][1][9] = 1;
    S[5][1][10] = 13;
    S[5][1][11] = 14;
    S[5][1][12] = 0;
    S[5][1][13] = 11;
    S[5][1][14] = 3;
    S[5][1][15] = 8;

    S[5][2][0] = 9;
    S[5][2][1] = 14;
    S[5][2][2] = 15;
    S[5][2][3] = 5;
    S[5][2][4] = 2;
    S[5][2][5] = 8;
    S[5][2][6] = 12;
    S[5][2][7] = 3;
    S[5][2][8] = 7;
    S[5][2][9] = 0;
    S[5][2][10] = 4;
    S[5][2][11] = 10;
    S[5][2][12] = 1;
    S[5][2][13] = 13;
    S[5][2][14] = 11;
    S[5][2][15] = 6;

    S[5][3][0] = 4;
    S[5][3][1] = 3;
    S[5][3][2] = 2;
    S[5][3][3] = 12;
    S[5][3][4] = 9;
    S[5][3][5] = 5;
    S[5][3][6] = 15;
    S[5][3][7] = 10;
    S[5][3][8] = 11;
    S[5][3][9] = 14;
    S[5][3][10] = 1;
    S[5][3][11] = 7;
    S[5][3][12] = 6;
    S[5][3][13] = 0;
    S[5][3][14] = 8;
    S[5][3][15] = 13;

    S[6][0][0] = 4;
    S[6][0][1] = 11;
    S[6][0][2] = 2;
    S[6][0][3] = 14;
    S[6][0][4] = 15;
    S[6][0][5] = 0;
    S[6][0][6] = 8;
    S[6][0][7] = 13;
    S[6][0][8] = 3;
    S[6][0][9] = 12;
    S[6][0][10] = 9;
    S[6][0][11] = 7;
    S[6][0][12] = 5;
    S[6][0][13] = 10;
    S[6][0][14] = 6;
    S[6][0][15] = 1;

    S[6][1][0] = 13;
    S[6][1][1] = 0;
    S[6][1][2] = 11;
    S[6][1][3] = 7;
    S[6][1][4] = 4;
    S[6][1][5] = 9;
    S[6][1][6] = 1;
    S[6][1][7] = 10;
    S[6][1][8] = 14;
    S[6][1][9] = 3;
    S[6][1][10] = 5;
    S[6][1][11] = 12;
    S[6][1][12] = 2;
    S[6][1][13] = 15;
    S[6][1][14] = 8;
    S[6][1][15] = 6;

    S[6][2][0] = 1;
    S[6][2][1] = 4;
    S[6][2][2] = 11;
    S[6][2][3] = 13;
    S[6][2][4] = 12;
    S[6][2][5] = 3;
    S[6][2][6] = 7;
    S[6][2][7] = 14;
    S[6][2][8] = 10;
    S[6][2][9] = 15;
    S[6][2][10] = 6;
    S[6][2][11] = 8;
    S[6][2][12] = 0;
    S[6][2][13] = 5;
    S[6][2][14] = 9;
    S[6][2][15] = 2;

    S[6][3][0] = 6;
    S[6][3][1] = 11;
    S[6][3][2] = 13;
    S[6][3][3] = 8;
    S[6][3][4] = 1;
    S[6][3][5] = 4;
    S[6][3][6] = 10;
    S[6][3][7] = 7;
    S[6][3][8] = 9;
    S[6][3][9] = 5;
    S[6][3][10] = 0;
    S[6][3][11] = 15;
    S[6][3][12] = 14;
    S[6][3][13] = 2;
    S[6][3][14] = 3;
    S[6][3][15] = 12;

    S[7][0][0] = 13;
    S[7][0][1] = 2;
    S[7][0][2] = 8;
    S[7][0][3] = 4;
    S[7][0][4] = 6;
    S[7][0][5] = 15;
    S[7][0][6] = 11;
    S[7][0][7] = 1;
    S[7][0][8] = 10;
    S[7][0][9] = 9;
    S[7][0][10] = 3;
    S[7][0][11] = 14;
    S[7][0][12] = 5;
    S[7][0][13] = 0;
    S[7][0][14] = 12;
    S[7][0][15] = 7;

    S[7][1][0] = 1;
    S[7][1][1] = 15;
    S[7][1][2] = 13;
    S[7][1][3] = 8;
    S[7][1][4] = 10;
    S[7][1][5] = 3;
    S[7][1][6] = 7;
    S[7][1][7] = 4;
    S[7][1][8] = 12;
    S[7][1][9] = 5;
    S[7][1][10] = 6;
    S[7][1][11] = 11;
    S[7][1][12] = 0;
    S[7][1][13] = 14;
    S[7][1][14] = 9;
    S[7][1][15] = 2;

    S[7][2][0] = 7;
    S[7][2][1] = 11;
    S[7][2][2] = 4;
    S[7][2][3] = 1;
    S[7][2][4] = 9;
    S[7][2][5] = 12;
    S[7][2][6] = 14;
    S[7][2][7] = 2;
    S[7][2][8] = 0;
    S[7][2][9] = 6;
    S[7][2][10] = 10;
    S[7][2][11] = 13;
    S[7][2][12] = 15;
    S[7][2][13] = 3;
    S[7][2][14] = 5;
    S[7][2][15] = 8;

    S[7][3][0] = 2;
    S[7][3][1] = 1;
    S[7][3][2] = 14;
    S[7][3][3] = 7;
    S[7][3][4] = 4;
    S[7][3][5] = 10;
    S[7][3][6] = 8;
    S[7][3][7] = 13;
    S[7][3][8] = 15;
    S[7][3][9] = 12;
    S[7][3][10] = 9;
    S[7][3][11] = 0;
    S[7][3][12] = 3;
    S[7][3][13] = 5;
    S[7][3][14] = 6;
    S[7][3][15] = 11;

    int l = 0;

    for (int i = 0; i < 8; ++i) {
        int b1 = subblock[6 * i];
        int b2 = subblock[6 * i + 1];
        int b3 = subblock[6 * i + 2];
        int b4 = subblock[6 * i + 3];
        int b5 = subblock[6 * i + 4];
        int b6 = subblock[6 * i + 5];

        int row = (b1 << 1) + b6;
        int column = (b2 << 3) + (b3 << 2) + (b4 << 1) + b5;

        int r = S[i][row][column];

        subblock[l + 3] = r & 1;
        r >>= 1;
        subblock[l + 2] = r & 1;
        r >>= 1;
        subblock[l + 1] = r & 1;
        r >>= 1;
        subblock[l] = r & 1;
        l += 4;
    }

    int sp[32] = {16, 7, 20, 21, 29, 12, 28, 17,
                  1, 15, 23, 26, 5, 18, 31, 10,
                  2, 8, 24, 14, 32, 27, 3, 9,
                  19, 13, 30, 6, 22, 11, 4, 25};

    vector<int> sm(32);

    for (int i = 0; i < 32; ++i)
        sm[i] = subblock[sp[i] - 1];

    subblock = sm;
}

void DES::crypt(vector<int> &block, vector<vector<int> > &keys) {
    vector<int> left(64);
    vector<int> right(64);

    this->initial_permutation(block);
    this->split(right, left, block, 32);

    vector<int> temp(64);
    for (int i = 1; i <= 16; i++) {
        temp = right;
        this->foo(temp, keys[i]);
        for (int i = 0; i < 32; ++i)
            temp[i] ^= left[i];

        left = right;
        right = temp;
    }

    this->merge(left, right, block, 32);
    this->final_permutation(block);
}

string DES::encode_message() {
    vector<char> en_message;
    vector<vector<int> > cryptBlock;

    for (int i = 0; i < message.size(); i += 8) {
        vector<int> block(64);

        char_to_bin_vector(message[i], block, 7);
        char_to_bin_vector(message[i + 1], block, 15);
        char_to_bin_vector(message[i + 2], block, 23);
        char_to_bin_vector(message[i + 3], block, 31);
        char_to_bin_vector(message[i + 4], block, 39);
        char_to_bin_vector(message[i + 5], block, 47);
        char_to_bin_vector(message[i + 6], block, 55);
        char_to_bin_vector(message[i + 7], block, 63);

        crypt(block, keys);

        cryptBlock.push_back(block);

        for (int q = 0; q < 64; q += 8) {

            vector<int> t;
            for (int j = q; j < q + 8; ++j) {
                t.push_back(block[j]);
            }

            en_message.push_back(char(bin_vector_to_char(t)));
        }
    }

    return string(en_message.begin(), en_message.end());
}

string DES::decode_message() {
    reverse(keys.begin() + 1, keys.end());
    return encode_message();
}