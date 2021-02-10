#ifndef SENSORS_H
#define SENSORS_H

#include <QString>
#include <QFile>
#include <QTextStream>

class Sensor
{
protected:
    QString name;
    QString location;

    float measrment;
    float lim;
    QString model;

    float prec;

    float eps;
    float F;

public:
    virtual QString GetType() const {return "";}

    virtual void SetMeasrment(const float) {}

    virtual void SetMeasrment(const float, const float) {}

    virtual float GetMeasrment() const {return 0;}

    virtual float GetLim() const {return 0;}

    virtual QString GetModel() const {return "";}

    virtual float GetPrec() const {return 0;}

    virtual float GetEps() const {return 0;}

    virtual float GetF() const {return 0;}

    virtual void SetLim(float) {}

    QString GetName() const {return name;}

    QString GetLocation() const {return location;}

    virtual void ReadFromFile(QTextStream &f)
    {
        f >> name >> location;
    }
};

class DD : public Sensor
{
public:
    void SetLim(float lim) override
    {
        this->lim = lim;
    }

    float GetLim() const override
    {
        return lim;
    }

    QString GetModel() const override
    {
        return model;
    }

    void SetMeasrment(const float measrment) override
    {
        this->measrment = measrment;
    }

    float GetMeasrment() const override
    {
        return measrment;
    }

    QString GetType() const override
    {
        return "DD";
    }

    void ReadFromFile(QTextStream &f) override
    {
        Sensor::ReadFromFile(f);
        f >> measrment >> lim >> model;
    }
};

class DT : public DD
{
public:
    float GetPrec() const override
    {
        return prec;
    }

    QString GetType() const override
    {
        return "DT";
    }

    void ReadFromFile(QTextStream &f) override
    {
        DD::ReadFromFile(f);
        f >> prec;
    }
};

class TD : public Sensor
{
public:
    void SetMeasrment(const float measrment1, const float measrment2) override
    {
        eps = measrment1;
        F = measrment2;
    }

    float GetEps() const override
    {
        return eps;
    }

    float GetF() const override
    {
        return F;
    }

    QString GetType() const override
    {
        return "TD";
    }

    void ReadFromFile(QTextStream &f) override
    {
        Sensor::ReadFromFile(f);
        f >> eps >> F;
    }
};

#endif // SENSORS_H
