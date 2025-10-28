/*
COP3502C | Fall 2025 | Section 0004
Name: Austin Vodrazka
UCF ID: 5524103
*/

int searchRecord(int numbers[], int low, int high, int value){
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (numbers[mid] == value) return mid;
    if (numbers[mid] > value) return searchRecord(numbers, low, mid - 1, value);
    return searchRecord(numbers, mid + 1, high, value);
}