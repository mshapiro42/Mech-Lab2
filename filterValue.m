function filteredValue = filterValue(inputs, outputs, b, a)
    sum = 0;
    for i=1:length(inputs)
        sum = sum + b(i)*inputs(i);
        if (i > 1)
            sum = sum - a(i)*outputs(i-1);
        end
    end
    sum = sum * 1/a(1);
    filteredValue = sum;
end
