#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

// 工具函数：检查两个二进制字符串是否只有一位不同
bool isOneBitDiff(const string& a, const string& b) {
    int diff = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) ++diff;
        if (diff > 1) return false;
    }
    return diff == 1;
}

// 工具函数：合并两个只有一位不同的二进制字符串
string merge(const string& a, const string& b) {
    string res = a;
    for (size_t i = 0; i < a.size(); ++i)
        if (a[i] != b[i]) res[i] = '-';
    return res;
}

// 输出表达式
string getExpression(const string& term, int varCount) {
    string expr;
    for (int i = 0; i < varCount; ++i) {
        if (term[i] == '1')
            expr += ('A' + i);
        else if (term[i] == '0')
            expr += ('a' + i); // 用小写表示取反，如a代表NOT A
    }
    if (expr.empty()) expr = "1"; // 恒为1项
    return expr;
}

int main() {
    int varCount;
    cout << "请输入变量个数: ";
    cin >> varCount;
    int mintermCount;
    cout << "请输入最小项个数: ";
    cin >> mintermCount;
    vector<int> minterms(mintermCount);
    cout << "请输入每个最小项（以空格分隔）: ";
    for (int i = 0; i < mintermCount; ++i) cin >> minterms[i];

    // Step 1: 将最小项转为二进制字符串
    vector<string> terms;
    for (int m : minterms) {
        string s(varCount, '0');
        for (int i = varCount - 1; i >= 0; --i) {
            s[i] = ((m >> (varCount - 1 - i)) & 1) ? '1' : '0';
        }
        terms.push_back(s);
    }

    // Q-M算法主循环
    set<string> primeImplicants;
    vector<string> current = terms, next;
    vector<bool> merged(current.size(), false);
    do {
        merged.assign(current.size(), false);
        next.clear();
        vector<bool> used(current.size(), false);
        for (size_t i = 0; i < current.size(); ++i) {
            bool found = false;
            for (size_t j = i + 1; j < current.size(); ++j) {
                if (isOneBitDiff(current[i], current[j])) {
                    string mg = merge(current[i], current[j]);
                    next.push_back(mg);
                    merged[i] = merged[j] = true;
                    found = true;
                }
            }
            if (!found && !merged[i]) primeImplicants.insert(current[i]);
        }
        // 去重
        sort(next.begin(), next.end());
        next.erase(unique(next.begin(), next.end()), next.end());
        current = next;
    } while (!current.empty());

    // 主项覆盖表
    map<string, set<int>> cover;
    for (const auto& pi : primeImplicants) {
        set<int> covered;
        for (int m : minterms) {
            bool match = true;
            for (int i = 0; i < varCount; ++i) {
                if (pi[i] != '-' && pi[i] != ((m >> (varCount - 1 - i)) & 1 ? '1' : '0')) {
                    match = false;
                    break;
                }
            }
            if (match) covered.insert(m);
        }
        cover[pi] = covered;
    }

    // 选取主项（最小覆盖），这里只做了简单的主项选择，未实现全覆盖最优选择（可扩展）
    set<int> uncovered(minterms.begin(), minterms.end());
    vector<string> essentialPrimeImplicants;
    while (!uncovered.empty()) {
        // 找覆盖未覆盖最小项最多的主项
        string best;
        int maxCount = 0;
        for (const auto& [pi, cov] : cover) {
            int cnt = 0;
            for (int m : cov)
                if (uncovered.count(m)) ++cnt;
            if (cnt > maxCount) {
                maxCount = cnt;
                best = pi;
            }
        }
        essentialPrimeImplicants.push_back(best);
        for (int m : cover[best]) uncovered.erase(m);
    }

    cout << "化简后的主项表达式：\n";
    for (size_t i = 0; i < essentialPrimeImplicants.size(); ++i) {
        if (i) cout << " + ";
        //cout << getExpression(essentialPrimeImplicants[i], varCount);
        cout << essentialPrimeImplicants[i];
    }
    cout << endl;
    return 0;
}