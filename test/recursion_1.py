def pow(x, n):
    if n == 1:
        return x

    if n % 2 == 1:
        return pow(x, n - 1) * x
    else:
        t = pow(x, n/2)
        return t * t

print pow(2, 7)
