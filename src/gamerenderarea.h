#ifndef GAMERENDERAREA_H
#define GAMERENDERAREA_H

#include <QPainter>
#include <QWidget>


class GameRenderArea : public QWidget {
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *event) override;
    [[nodiscard]] QSize sizeHint() const override;

public:
    explicit GameRenderArea(QWidget *parent = nullptr);

    void display(std::vector<std::vector<bool>> display);
    void offColor(QColor offColor);
    void onColor(QColor onColor);

private:
    QColor offColor_;
    QColor onColor_;
    std::vector<std::vector<bool>> display_;
    int pixelSize_;
};

#endif // GAMERENDERAREA_H
