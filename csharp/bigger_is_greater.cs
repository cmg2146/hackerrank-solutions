// Solves: https://www.hackerrank.com/challenges/bigger-is-greater/problem?isFullScreen=true

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

    public static int? GetNextGreatestCharToRight(string w, int index)
    {
        int? result = null;
        var curChar = w[index];

        for (int i = index+1; i < w.Length; i++)
        {
            var testChar = w[i];
            if (testChar > curChar)
            {
                if ((!result.HasValue)
                    || (testChar.CompareTo(w[result.Value]) < 0))
                {
                    result = i;
                }
            }
        }

        return result;
    }

    /*
     * Complete the 'biggerIsGreater' function below.
     *
     * The function is expected to return a STRING.
     * The function accepts STRING w as parameter.
     */

    public static string biggerIsGreater(string w)
    {
        int? targetChar = null;
        int? charToSwap = null;

        // get the last character in the string for which there is a character to the right of it
        // with a greater sorting value
        for (int i = 0; i < w.Length; i++)
        {
            // for the current character in the string, try to get the next greatest character
            // to the right of it
            var nextGreatestChar = GetNextGreatestCharToRight(w, i);
            if (nextGreatestChar.HasValue)
            {
                targetChar = i;
                charToSwap = nextGreatestChar;
            }
        }
        
        if (!targetChar.HasValue || !charToSwap.HasValue)
        {
            return "no answer";
        }

        // move the next greatest char just in front of the target char
        // then sort the remainder of the string starting at the target char
        var prefix = w[..targetChar.Value] + w[charToSwap.Value];
        var postfix = w.Remove(charToSwap.Value, 1);
        postfix = string.Join("", postfix[targetChar.Value..].Order());
        
        return prefix + postfix;
    }
}