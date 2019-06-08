#ifndef GLWORKER_H
#define GLWORKER_H

#include <QObject>
#include <vector>
#include <utility>
#include <vector>

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
private:
    double distance_pow2(double, double, double, double);
    bool equal(double, double, double, double);
    double a_tan2(double, double, bool);
    std::vector<bool> used_points;
    constexpr static double eps = 0.0001;
};

#endif // GLWORKER_H
