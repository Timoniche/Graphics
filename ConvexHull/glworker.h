#ifndef GLWORKER_H
#define GLWORKER_H

#include <QObject>
#include <vector>
#include <utility>
#include <vector>

struct StackExtended
{
public:
    void push(size_t x);
    int top();
    int top_next();
    int pop();
public:
    std::vector<size_t> _data;
};

class GLWorker : public QObject
{
    Q_OBJECT
public:
    GLWorker();
    typedef std::vector<std::pair<float, float>> data_t;
public slots:
    void gift_wrapping(data_t const &);
    void graham(data_t const &);
signals:
    void send_line(float, float, float, float);
    void send_square(float);
private:
    constexpr static float eps = float(0.000001);
    float distance_pow2(float, float, float, float) const;
    bool left_directed(size_t, size_t, size_t, data_t const&) const;
    bool equal(float, float) const;
    bool equal(float, float, float, float) const;
    float a_tan2(float, float, bool) const;
    float a_tan2_from_0_to_pi(float, float) const;
    std::vector<bool> used_points;
private:
    StackExtended _stack;
};

#endif // GLWORKER_H
