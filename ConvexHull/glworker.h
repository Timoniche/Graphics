#ifndef GLWORKER_H
#define GLWORKER_H

#include <QObject>
#include <vector>
#include <utility>

class GLWorker : public QObject
{
    Q_OBJECT
public:
    GLWorker();
    typedef std::vector<std::pair<double, double>> data_t;
public slots:
    void build_hull(data_t const&);
signals:
    void send_line(double, double, double, double);
};

#endif // GLWORKER_H
