function outputs = filterValue(newInput, inputs, outputs, b, a)
    inputs = [newInput, inputs(1:4)];
    sum = 0;
    for i=1:5
        sum = sum + b(i)*inputs(i);
        if (i > 1)
            sum = sum - a(i)*outputs(i);
        end
    end
    sum = sum * 1/a(1);
    outputs = [sum, outputs(1:4)];
end
