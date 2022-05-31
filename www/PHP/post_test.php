 <!DOCTYPE html>
<html>
<body>

<h1>Super</h1>

Bonjour, <?php echo htmlspecialchars($_POST['nom']); ?>.
Tu as <?php echo (int)$_POST['age']; ?> ans.

</body>
</html> 
