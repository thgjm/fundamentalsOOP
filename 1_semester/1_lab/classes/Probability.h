#include <iostream>
#include <vector>
#include <cmath>

class RandomEvent
{
protected:
  std::string name;

public:
  RandomEvent(const std::string &name) : name(name) {}

  std::string getName() const
  {
    return name;
  }

  virtual double probability() const = 0;
};

class RandomVariable : public RandomEvent
{
protected:
  std::vector<double> values;

public:
  RandomVariable(const std::string &name, const std::vector<double> &values)
      : RandomEvent(name), values(values) {}

  std::vector<double> getValues() const
  {
    return values;
  }

  virtual double expectedValue() const = 0;
  virtual double variance() const = 0;
};

class DiscreteRandomVariable : public RandomVariable
{
protected:
  std::vector<double> probabilities;

  // used in child classes
  double factorial(int num) const
  {
    if (num == 0 || num == 1)
      return 1;
    return num * factorial(num - 1);
  }

public:
  DiscreteRandomVariable(const std::string &name, const std::vector<double> &values, const std::vector<double> &probabilities)
      : RandomVariable(name, values), probabilities(probabilities)
  {
    if (values.size() != probabilities.size())
    {
      std::cout << "Error. The number of values and probabilities should be equal." << std::endl;
      this->values.clear();
      this->probabilities.clear();
      return;
    }
  }

  double probability() const override // should be 1
  {
    double totalProbability = 0.0;
    for (const auto &prob : probabilities)
      totalProbability += prob;
    return totalProbability;
  }

  double expectedValue() const override
  {
    double expValue = 0.0;
    for (auto i = 0; i < values.size(); i++)
      expValue += values[i] * probabilities[i];
    return expValue;
  }

  double variance() const override
  {
    double expValue = expectedValue();
    double expValueSquare = 0.0;
    for (auto i = 0; i < values.size(); i++)
      expValueSquare += (values[i] * values[i]) * probabilities[i];

    double var = expValueSquare - (expValue * expValue);

    return var;
  }

  void printOutDistribution()
  {
    for (auto i = 0; i < values.size(); i++)
      std::cout << "P(ξ = " << values[i] << ") = " << probabilities[i] << "\n";
  }

  std::vector<double> getProbabilities() const
  {
    return probabilities;
  }
};

class BinomialDistribution : public DiscreteRandomVariable
{
private:
  int n;    // number of trials
  double p; // probability of success
public:
  BinomialDistribution(const std::string &name, int n, double p)
      : DiscreteRandomVariable(name, {}, {})
  {
    if (n < 0 || p < 0 || p > 1)
    {
      std::cout << "Error. Invalid parameters." << std::endl;
      this->n = 0;
      this->p = 0.0;
      return;
    }
    this->n = n;
    this->p = p;
    calculateDistribution();
  }

  void calculateDistribution() // distribution for ξ = k, where k=[0, n]
  {
    values.clear();
    probabilities.clear();

    for (int k = 0; k <= n; k++)
    {
      double probability = binomialProbability(n, k, p);
      values.push_back(k);
      probabilities.push_back(probability);
    }
  }

  double binomialProbability(int n, int k, double p) const
  {
    // binomial distribution formula
    double combination = factorial(n) / (factorial(k) * factorial(n - k));
    return combination * pow(p, k) * pow(1 - p, n - k);
  }

  double expectedValue() const override
  {
    return n * p;
  }

  double variance() const override
  {
    return n * p * (1 - p);
  }
};

class PoissonDistribution : public DiscreteRandomVariable
{
private:
  int k;
  double lambda; // lambda>0
public:
  PoissonDistribution(const std::string &name, int k, double lambda)
      : DiscreteRandomVariable(name, {}, {})
  {
    if (lambda <= 0 || k < 0)
    {
      std::cout << "Error. Invalid parameters." << std::endl;
      this->k = 0;
      this->lambda = 0.0;
      return;
    }
    this->k = k;
    this->lambda = lambda;
    calculateDistribution();
  }

  void calculateDistribution() // distribution for ξ = k
  {
    values.clear();
    probabilities.clear();

    double probability = poissonProbability(k, lambda);
    values.push_back(k);
    probabilities.push_back(probability);
  }

  double poissonProbability(int k, double lambda) const
  {
    // poisson distribution formula
    return pow(lambda, k) * exp(-lambda) / factorial(k);
  }

  double expectedValue() const override
  {
    return lambda;
  }

  double variance() const override
  {
    return lambda;
  }
};

// measures the probability of success after a given number of trials.
class GeometricDistribution : public DiscreteRandomVariable
{
private:
  int k;    // number of trials
  double p; // probability of success
public:
  GeometricDistribution(const std::string &name, int k, double p)
      : DiscreteRandomVariable(name, {}, {})
  {
    if (k < 0 || p < 0 || p > 1)
    {
      std::cout << "Error. Invalid parameters." << std::endl;
      this->k = 0;
      this->p = 0.0;
      return;
    }
    this->k = k;
    this->p = p;
    calculateDistribution();
  }

  void calculateDistribution() // distribution for ξ = k
  {
    values.clear();
    probabilities.clear();

    double probability = geometricProbability(k, p);
    values.push_back(k);
    probabilities.push_back(probability);
  }

  double geometricProbability(int k, double lambda) const
  {
    // geometric distribution formula
    return pow(1 - p, k - 1) * p;
  }

  double expectedValue() const override
  {
    return 1 / p;
  }

  double variance() const override
  {
    return (1 - p) / (p * p);
  }
};