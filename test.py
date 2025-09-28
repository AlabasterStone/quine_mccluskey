# 从result.out中读取化简后的表达式
with open('result.out', 'r', encoding='utf-8') as f:
    exp = f.read().strip()
exp = exp.replace(' ','')
print(exp)

def unit(s: str, input: list[bool]):
    i=0
    res=1    
    for c in s:
        if c=='-':
            pass
        elif c=='0':
            res = res and (not input[i])
        elif c=='1':
            res = res and input[i]
        i+=1
    return res

n = 10
# 一个n位数，每一位为0或1，用程序枚举所有的组合情况
m = 0
minterms = []
for i in range(2**n):
    s = bin(i)[2:]  # 转化为二进制字符串
    s = '0' * (n - len(s)) + s  # 前面补0
    input = [c == '1' for c in s]
    res = 0
    for c in exp.split('+'):
        res = res or unit(c, input)
    print(f'{s} | {str(int(res))}')
    if res:
        m += 1
        minterms.append(i)
print(minterms)
print(m)