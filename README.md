# Challenge: Build a “Starter Bundle” (3 products)
You run a small e-commerce shop. Using the dataset below, pick exactly 3 products to feature as a “Starter Bundle” for next month.

## Goal
Select 3 products that maximize a Bundle Score while satisfying constraints.

### Constraints
At least 2 distinct categories across the 3 products.

Each chosen product must have:

* Return rate ≤ 0.20 (based on units returned / units sold)

* Average rating ≥ 4.0

* Net revenue > 0

Tie-breakers (in order):

higher net revenue > lower return rate > higher average rating > alphabetical by product_name.

### Metrics you must compute (per product)

units_sold = sum(qty)

units_returned = sum(qty_returned)

gross_revenue = sum(qty * unit_price)

discount_amount = sum(qty * unit_price * discount_pct)

refund_amount = sum(qty_returned * unit_price * (1 - discount_pct))
(refund is based on discounted unit price)

net_revenue = gross_revenue - discount_amount - refund_amount

return_rate = units_returned / units_sold (treat 0/0 as 0)

avg_rating = average(rating) over all order lines for that product (ignore blank ratings)

Bundle Score (per product)

Compute:

score =
  0.55 * normalize(net_revenue) +
  0.25 * normalize(avg_rating) +
  0.20 * normalize(1 - return_rate)

Where normalize(x) is min-max scaling across products:
(x - minX) / (maxX - minX), and if maxX == minX use 0.

Then select the best 3-product bundle under the constraints, maximizing the sum of scores.