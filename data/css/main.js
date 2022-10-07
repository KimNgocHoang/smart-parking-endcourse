var refInterval = window.setInterval('update()', 1000); // 30 seconds


var update = function() {
    $.ajax({
        type : 'POST',
        url : '/',
        success : function(data){
            $('#state').html(data);
        },
    });
};
update();