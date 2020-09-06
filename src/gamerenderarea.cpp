#include "gamerenderarea.h"
#include "emulator.h"


GameRenderArea::GameRenderArea(QWidget *parent)
    : QWidget(parent)
    , offColor_(QColor(0, 0, 0))
    , onColor_(QColor(255, 255, 255))
    , pixelSize_(10) { }

void GameRenderArea::paintEvent(QPaintEvent *) {
    if (display_.size() > 0) {
        QPainter painter(this);

        const size_t rows = display_.size();
        const size_t cols = rows > 0 ? display_[0].size() : 0;

        for (size_t rowIdx = 0; rowIdx < rows; ++rowIdx) {
            for (size_t colIdx = 0; colIdx < cols; ++colIdx) {
                int x = static_cast<int>(colIdx) * pixelSize_;
                int y = static_cast<int>(rowIdx) * pixelSize_;

                if (display_[rowIdx][colIdx])
                    painter.fillRect(x, y, pixelSize_, pixelSize_, onColor_);
                else
                    painter.fillRect(x, y, pixelSize_, pixelSize_, offColor_);
            }
        }

        update();
    }
}

QSize GameRenderArea::sizeHint() const {
    QSize size(pixelSize_ * Emu::Constants::displayCols, pixelSize_ * Emu::Constants::displayRows);

    return size;
}

void GameRenderArea::display(std::vector<std::vector<bool>> display) {
    this->display_ = display;
}

void GameRenderArea::pixelSize(int pixelSize) {
    this->pixelSize_ = pixelSize;
}

void GameRenderArea::offColor(QColor offColor) {
    this->offColor_ = offColor;
}

void GameRenderArea::onColor(QColor onColor) {
    this->onColor_ = onColor;
}
