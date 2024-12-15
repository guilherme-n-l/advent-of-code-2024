using LinearAlgebra

const FACTOR = 1e13

function get_input()
    open("input2.txt", "r") do f
        nums = []
        for l in eachline(f)
            push!(nums, map(x -> parse(Float64, x), split(l)))
        end
        return nums
    end
end

function is_valid_solution(x)
    is_valid = true
    for i in x
        if abs(i - round(i)) > 1e-3 || i < 0
            is_valid = false
            break
        end
    end
    return is_valid
end

input = get_input()

global sum_ = 0

for i in input
    A = [i[1] i[3]; i[2] i[4]]
    b = [i[5]; i[6]]

    if rank(A) != size(A, 1)
        continue
    end

    x = (A / FACTOR) \ (b / FACTOR)

    if !is_valid_solution(x)
        continue
    end

    global sum_ += sum([3 * round(x[1]) round(x[2])])
end

println(Int(sum_))
