import math

def main():
    is_sym, is_tsp, need_path = True, True, True
    for n in range(3, 33 + 1):
        bhk = exact_complexity(n, is_sym,  is_tsp, need_path)
        print(
            f'N = {n}, no. of BHK steps: {bhk}'
            f' VS num perms: {math.factorial(n - int(is_sym))}'
        )

def exact_complexity(
    n: int,
    is_sym=True,
    is_cyclic=True,
    need_path=True,
    is_cold=True
) -> int:
    if is_cyclic:
        n -= 1
    max_card = max(1, n // 2) if is_sym else n - 1
    total = sum([
        math.comb(n, k) * (n - k) * k
        for k in range(1, max_card)
    ])
    factor = 0.5 if is_sym and n % 2 == 0 else 1
    if is_sym:
        total += factor * sum([
            math.comb(n, k) * (n - k) * (k + ((n - k) if n & 1 else 0))
            for k in range(max_card, max_card  + 1)
        ])
    else:
        total += factor * sum([
            math.comb(n, k) * (n - k) * k
            for k in range(max_card, max_card  + 1)
        ])
    if is_cold:  # for init
        total += n + (n + 1) * max_card
    if need_path:
        total += n + n - 1  # reconsructing solution
    return total


if __name__ == "__main__":
    main()
