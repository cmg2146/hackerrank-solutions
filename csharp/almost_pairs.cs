// Solves: https://www.hackerrank.com/challenges/pairs/problem?isFullScreen=true

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
     * Complete the 'pairs' function below.
     *
     * The function is expected to return an INTEGER.
     * The function accepts following parameters:
     *  1. INTEGER k
     *  2. INTEGER_ARRAY arr
     */

    public static int pairs(int k, List<int> arr)
    {
        int result = 0;

        var arrAsDict = arr.ToDictionary(num => num);
        foreach (var entry in arrAsDict)
        {
            var pairedNumber = entry.Key - k;
            if (arrAsDict.ContainsKey(pairedNumber))
            {
                result += 1;
            }           
        }

        return result;
    }
}