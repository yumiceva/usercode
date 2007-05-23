// Formatted last-modified date
if (Date.parse(document.lastModified) != 0) {
  var modiDate = new Date(document.lastModified);
  var monthName = new Array("January", "February", "March", "April", "May", 
   "June", "July", "August", "September", "October", "November", "December");
  var dateGMT = modiDate.toLocaleString();
  document.write("Last modified: " + monthName[modiDate.getMonth()] + " ");
  document.write(modiDate.getDate() + ", " + modiDate.getFullYear() + " " + dateGMT.substring(dateGMT.indexOf(':')-2,dateGMT.indexOf(':')+6));
  }
