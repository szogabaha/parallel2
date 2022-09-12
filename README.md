# Numerical integration using trapezoids


The integral calculation is fair, so we do need to load balance the work of the threads since each of them compute the same value

```
$: ./numerical_integration -threads T -trapezoids N
```