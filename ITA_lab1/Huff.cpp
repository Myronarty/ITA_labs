#include "Huff.h"

struct HuffNode
{
    uint8_t symbol;
    uint32_t freq;
    HuffNode* left;
    HuffNode* right;

    HuffNode(uint8_t s, uint32_t f) : symbol(s), freq(f), left(nullptr), right(nullptr) {}
};

struct CompareHuffNode
{
    bool operator()(HuffNode* l, HuffNode* r)
    {
        return l->freq > r->freq;
    }
};

void GenerateCodes(HuffNode* root, uint64_t currentCode, int length, vector<pair<uint64_t, int>>& codes)
{
    if (!root) return;

    if (!root->left && !root->right)
    {
        if (length == 0) length = 1;
        codes[root->symbol] = { currentCode, length };
        return;
    }

    GenerateCodes(root->left, (currentCode << 1), length + 1, codes);
    GenerateCodes(root->right, (currentCode << 1) | 1, length + 1, codes);
}

void FreeTree(HuffNode* root)
{
    if (!root) return;
    FreeTree(root->left);
    FreeTree(root->right);
    delete root;
}

HuffNode* BuildHuffmanTree(const vector<uint32_t>& freqs)
{
    priority_queue<HuffNode*, vector<HuffNode*>, CompareHuffNode> pq;

    for (int i = 0; i < 256; i++)
    {
        if (freqs[i] > 0)
        {
            pq.push(new HuffNode(static_cast<uint8_t>(i), freqs[i]));
        }
    }

    if (pq.empty()) return nullptr;

    while (pq.size() > 1)
    {
        HuffNode* left = pq.top(); pq.pop();
        HuffNode* right = pq.top(); pq.pop();

        HuffNode* parent = new HuffNode(0, left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    return pq.top();
}

void CodeHuffman(string name_orig, string name_encode)
{
    ifstream inFile(name_orig, ios::binary);
    if (!inFile.is_open())
    {
        cerr << "Error opening input file." << endl;
        return;
    }

    vector<uint32_t> freqs(256, 0);
    char ch;
    while (inFile.get(ch))
    {
        freqs[static_cast<uint8_t>(ch)]++;
    }

    fstream outFile(name_encode, ios::binary | ios::out | ios::trunc);
    if (!outFile.is_open())
    {
        cerr << "Error creating output file." << endl;
        inFile.close();
        return;
    }
    outFile.write(reinterpret_cast<const char*>(freqs.data()), freqs.size() * sizeof(uint32_t));

    HuffNode* root = BuildHuffmanTree(freqs);
    if (!root)
    {
        inFile.close();
        outFile.close();
        return;
    }

    vector<pair<uint64_t, int>> codes(256, { 0, 0 });
    GenerateCodes(root, 0, 0, codes);

    BitWorker bw(outFile);
    inFile.clear();
    inFile.seekg(0, ios::beg);

    while (inFile.get(ch))
    {
        uint8_t symbol = static_cast<uint8_t>(ch);
        bw.WriteBitSequence(codes[symbol].first, codes[symbol].second);
    }

    bw.flush();

    FreeTree(root);
    inFile.close();
    outFile.close();
    cout << "Huffman compression successful: " << name_encode << endl;
}

void deCodeHuffman(string name_encode, string name_rez)
{
    ifstream inFile(name_encode, ios::binary);
    if (!inFile.is_open())
    {
        cerr << "Error opening compressed file." << endl;
        return;
    }

    vector<uint32_t> freqs(256, 0);
    inFile.read(reinterpret_cast<char*>(freqs.data()), 256 * sizeof(uint32_t));

    uint64_t totalSymbols = 0;
    for (uint32_t f : freqs)
    {
        totalSymbols += f;
    }

    if (totalSymbols == 0)
    {
        ofstream outFile(name_rez, ios::binary);
        inFile.close();
        outFile.close();
        return;
    }

    HuffNode* root = BuildHuffmanTree(freqs);
    HuffNode* current = root;

    ofstream outFile(name_rez, ios::binary);

    uint64_t decodedCount = 0;
    uint8_t buffer;

    while (inFile.read(reinterpret_cast<char*>(&buffer), 1) && decodedCount < totalSymbols)
    {
        for (int i = 7; i >= 0; i--)
        {
            bool bit = (buffer >> i) & 1;

            if (bit) current = current->right;
            else current = current->left;

            if ((!current->left) && (!current->right))
            {
                outFile.put(static_cast<char>(current->symbol));
                decodedCount++;
                current = root;

                if (decodedCount >= totalSymbols)
                {
                    break;
                }
            }
        }
    }

    FreeTree(root);
    inFile.close();
    outFile.close();
    cout << "Huffman decompression successful: " << name_rez << endl;
}
