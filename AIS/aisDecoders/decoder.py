from pyais import decode

arquivo_entrada = "AIS/nmea0183.txt"
arquivo_saida = "AIS/saida_decodificada.txt"

with open(arquivo_entrada, "r") as f, open(arquivo_saida, "w") as out:
    for linha in f:
        linha = linha.strip()
        if not linha:
            continue  
        try:
            mensagemDecodificada = decode(linha)
            parametros = mensagemDecodificada.asdict()
            
            out.write("\n--- Nova mensagem ---\n")
            for chave, valor in parametros.items():
                out.write(f"{chave}: {valor}\n")
        
        except Exception as e:
            out.write(f"\nErro ao decodificar linha: {linha}\n")
            out.write(f"Detalhe do erro: {e}\n")