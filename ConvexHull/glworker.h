#ifndef GLWORKER_H
#define GLWORKER_H

#include <QObject>
#include <vector>
#include <utility>
#include <vector>

#include "hulllib.h"

class GLWorker : public QObject
{
    Q_OBJECT
public:
    GLWorker();
    typedef std::vector<Point> vector_of_points;
public slots:
    void gift_wrapping(vector_of_points const &);
    void graham(vector_of_points const &);
signals:
    void send_line(float, float, float, float);
    void send_vector_lines(vector_of_points const&);
    void send_square(float);
    void send_max_bar(size_t);
    void send_bar_value(size_t);
    void increase_bar(size_t);
private:
    std::vector<bool> used_points;
private:
    HullLib _lib;
};

#endif // GLWORKER_H
