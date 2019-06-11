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
    typedef std::vector<std::pair<float, float>> data_t;
public slots:
    void gift_wrapping(data_t const&);
signals:
    void send_line(float, float, float, float);
private:
    float distance_pow2(float, float, float, float);
    bool equal(float, float, float, float);
    float a_tan2(float, float, bool);
    std::vector<bool> used_points;
    constexpr static float eps = 0.0001;
};

#endif // GLWORKER_H
