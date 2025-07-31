window.addEventListener('DOMContentLoaded', () => {
  const params = new URLSearchParams(window.location.search);

  const order = params.get("order");
  const start = params.get("start");
  const end = params.get("end");

  if (order !== null) {
    document.getElementById('order').value = order;
  }

  if (start) {
    document.getElementById('start-date').value = start;
  }

  if (end) {
    document.getElementById('end-date').value = end;
  }
});

document.getElementById('filter-form').addEventListener('submit', function(e) {
  e.preventDefault();

  const order = document.getElementById('order').value;
  const startDate = document.getElementById('start-date').value;
  const endDate = document.getElementById('end-date').value;

  const params = new URLSearchParams();

  if (order) params.set("order", order);
  if (startDate) params.set("start", startDate);
  if (endDate) params.set("end", endDate);

  window.location.href = `?${params.toString()}`;
});
