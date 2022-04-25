#pragma once


struct LinearGeneratorParams 
{
public:
	int width;
	int height;

	float powerFill;
	int numIter;
	int timeIterLight;

	int choiseLocation;

	int degP;
	int degP2;

	float Spreading;
};


class LinearGeometryGenerator
{
private:
	LinearGeneratorParams param;
/*protected:
	virtual void GenerateLinearMap(IntVector2& startPos, IntVector2& endPos) override;*/
public:
	LinearGeometryGenerator* Init(const LinearGeneratorParams& params);
};