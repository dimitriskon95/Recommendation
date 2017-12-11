#ifndef Heuristic_H
#define Heuristic_H

template <class T>
class Heuristic
{
	private:
		T* hfunctions;  //int for hamming , double for euclidean or cosine (= vector)
		int dims;       // = k for hamming //size of hfunctions 
		char metric;
		double t;
	public:
		Heuristic();
		~Heuristic();
		
		void Heuristic_Init(char, int);
		
		void Hamming_Random(int);
		void Euclidean_Random(int);
		void Cosine_Random();
		void DistanceMatrix_Random(int);
		
		T Hamming_Hreturn(int);

		
		T get_t();
		T* get_hfunctions();
		
	//	void set_median(double);
};

#endif

