#include "Singleton.h"

namespace
{
	constexpr int kMaxFinalizerSize = 256;
	std::mutex mutex;
	int num_finalizer_size = 0;
	SingletonFinalizer::FinalizerFunc finalizers[kMaxFinalizerSize];
}

void SingletonFinalizer::AddFinalizer(FinalizerFunc func)
{
	std::lock_guard<std::mutex> lock(mutex);
	finalizers[num_finalizer_size++] = func;
}

void SingletonFinalizer::Finalize()
{
	std::lock_guard<std::mutex> lock(mutex);
	for (int i = num_finalizer_size - 1; i >= 0; --i)
	{
		(finalizers[i]());
	}
	num_finalizer_size = 0;
}
