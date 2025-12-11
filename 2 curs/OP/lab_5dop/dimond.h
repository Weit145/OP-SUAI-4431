class Diamond : public Shape {
    Point leftTop, rightBottom;
public:
    Diamond(Point lt, Point rb) : leftTop(lt), rightBottom(rb) {}

    void draw(Screen* screen) const override {
        int midX = (leftTop.getX() + rightBottom.getX()) / 2;
        int topY = leftTop.getY();
        int bottomY = rightBottom.getY();

        for (int y = topY; y <= bottomY; y++) {
            int offset = (y - topY <= bottomY - y ? y - topY : bottomY - y);
            for (int x = leftTop.getX() + offset; x <= rightBottom.getX() - offset; x++) {
                screen->putPoint(x, y);
            }
        }
    }



    void move(Point p) override {
        // смещение ромба на точку p
        int dx = p.getX() - leftTop.getX();
        int dy = p.getY() - leftTop.getY();
        leftTop.setX(leftTop.getX() + dx);
        leftTop.setY(leftTop.getY() + dy);
        rightBottom.setX(rightBottom.getX() + dx);
        rightBottom.setY(rightBottom.getY() + dy);
    }

    Point getLeftTop() const override { return leftTop; }
    Point getRightTop() const override { return Point(rightBottom.getX(), leftTop.getY()); }
    Point getLeftBottom() const override { return Point(leftTop.getX(), rightBottom.getY()); }
    Point getRightBottom() const override { return rightBottom; }
};
