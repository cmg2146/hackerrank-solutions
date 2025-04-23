// Solves: https://www.hackerrank.com/challenges/non-divisible-subset/problem?isFullScreen=true

using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Text.RegularExpressions;
using System.Text;
using System;

class Result
{

    /*
     * Complete the 'nonDivisibleSubset' function below.
     *
     * The function is expected to return an INTEGER.
     * The function accepts following parameters:
     *  1. INTEGER k
     *  2. INTEGER_ARRAY s
     */

    public static int nonDivisibleSubset(int k, List<int> s)
    {
        if ((s.Count < 2) || (k == 1))
        {
            return 1;
        }
        
        int result = 0;

        // for each remainder 0 through k-1, get the count of numbers in the Set s
        // that have the corresponding remainder when mod'd with k.
        List<int> remainderCount = Enumerable.Repeat(0, k).ToList();    
        foreach (var num in s)
        {
            var remainder = num % k;
            remainderCount[remainder]++;
        }
        
        // to get the size of the desierd maximal set, we will look through the remainder
        // count list. The permuations that will not work are: adding numbers with remainder
        // count 1 and remainder count k-1, remainder count 2 and k-2, and so on.
        for (int i = 1, j = k-1; j > i; i++, j--)
        {
            result += Math.Max(remainderCount[i], remainderCount[j]);
        }
        
        if ((k % 2) == 0)
        {
            // when k is even, numbers with remainder count k/2 can not be added together, so
            // we can take at most 1
            result += Math.Min(1, remainderCount[k/2]);
        }
        
        // dont forget to include the numbers that are divisible by k, but no more than one of them because they cant be added together either
        result += Math.Min(1, remainderCount[0]);
        
        return result;
    }
}
