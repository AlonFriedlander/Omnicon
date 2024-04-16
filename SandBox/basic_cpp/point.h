#ifndef POINT_H
#define POINT_H

struct point
{
    int x;
    int y;

    void AdjustBy(const point &other);
    void AdjustBy(int dx, int dy);
    
    enum ParenthesesType{ROUND, SQUARE,CURLY,ANGULAR};
    void print (ParenthesesType parenthesesType) const;
    void print (char open, char close) const;
    double length() const;

    char GetOpeningParentheses(ParenthesesType parenthesesType) const;
    char GetClosingParentheses(ParenthesesType parenthesesType) const;
};

point Add(const point &p1, const point &p2);

#endif
