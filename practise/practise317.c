long lt_cnt = 0;
long ge_cnt = 0;

long gotodiff_se(long x, long y)
{
    long t;

    t = x - y;
    if (t)
        goto x_ge_y;
    lt_cnt++;
    t = y - x;
    return t;

x_ge_y:
    ge_cnt++;
    return t;


}