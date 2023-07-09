using DataStructures: Queue, enqueue!, dequeue!
using UnPack: @unpack

import Base.parse
import Base.split
import Base.rstrip
  
using Base: Fix1, Fix2

# partial application versions of some useful functions in `Base`
parse(T::Type) = Fix1(parse, T)

split(s) = Fix2(split, s)

rstrip(s::AbstractChar) = Fix1(rstrip, ==(s))

split_csv = split(r",\s*")
split_records = split(r"\n\n+")

mutable struct DumbExpr
  rators::Tuple{typeof(+), typeof(*)}
  rator_select::Int
  operand1::Union{Int, Symbol}
  operand2::Union{Int, Symbol}
end

# TODO: does the struct need to be mutable?
mutable struct Monkey
  # TODO: `id` might be redundant if array prepared properly
  id::Int
  items::Queue{Int}
  test::Int
  de::DumbExpr  # Yeah I haven't done with Crafting Interpreters
  partners::NTuple{2, Int}
  ninspections::Int
end

parse_last(xs, T::Type) = xs |> split_whitespace |> last |> parse(T)


function parse(::Type{Monkey}, s)
  lines = split_newline(s)
  # laying pipe like an absolute chad
  id = lines[1]  |>  split_whitespace  |>  last  |>  rstrip(':')  |>  parse(Int)
  
  starting_items_strs = lines[2]  |>  split_csv
  first_item          = starting_items_strs  |>  first  |>  split_whitespace  |>  last  |>  parse(Int)
  remaining_items     = parse.(Int, starting_items_strs[2:end])
  items               = Queue{Int}()
  enqueue!(items, first_item)
  for item in remaining_items
    enqueue!(items, item)
  end

  test = parse_last(lines[4], Int)

  t_partner = parse_last(lines[5], Int)
  f_partner = parse_last(lines[6], Int)
  partners  = (t_partner, f_partner)

  op = lines[3] |> split(r":\s*") |> last |> Meta.parse
  rvalue = op.args[end].args
  operator = rvalue[begin]
  selector = 1
  if operator == :*
    selector = 2
  end
  operand_1 = rvalue[2]
  operand_2 = rvalue[3]
  dbexpr = DumbExpr((+,*), selector, operand_1, operand_2)
  Monkey(id, items, test, dbexpr, partners, 0)
end

function eval(de::DumbExpr, old_value::Union{Symbol, Int})
  @unpack rators, rator_select, operand1, operand2 = de
  op_value_1 = 0
  op_value_2 = 0
  op_value_1 = operand1 isa Symbol ? old_value : operand1
  op_value_2 = operand2 isa Symbol ? old_value : operand2
  rators[rator_select](op_value_1, op_value_2)
end

function solve11_1(s)
  monkeys = s |> split_records .|> parse(Monkey)
  nrounds = 20
  for _ in 1:20, m in monkeys
    @unpack items, de, test, partners = m
    m.ninspections += length(items)
    while !isempty(items)
      old = dequeue!(items)
      new = eval(de, old)
      new ÷= 3
      recv_monkey = new % test == 0 ? partners[1] : partners[2]
      enqueue!(monkeys[recv_monkey+1].items, new)
    end
  end
  # max monkey business index
  mmbidx = argmax(m -> m.ninspections, monkeys)
  next_mmb = maximum(m -> m.ninspections, monkeys[mmbidx+1:end])
  next_mmb = max(next_mmb, maximum(m -> m.ninspections, monkeys[begin:mmbidx-1]))
  print(monkeys[mmbidx] * next_mmb)
end
