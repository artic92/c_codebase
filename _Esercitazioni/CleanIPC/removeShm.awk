{
 if (FNR >3)
  if ($2 != null)
     system("ipcrm -m" $2)

}
